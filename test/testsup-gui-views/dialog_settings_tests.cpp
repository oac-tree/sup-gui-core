/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sup/gui/core/message_event.h>
#include <sup/gui/mainwindow/settings_editor_dialog.h>
#include <sup/gui/model/persistent_settings_model.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/detailed_message_box.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for dialogs and views storing their settings in the settings manager.
 */
class DialogSettingsTest : public ::testing::Test
{
public:
  DialogSettingsTest() { m_manager.SetStorage(SettingsScope::kApplication, &m_storage); }

  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
};

TEST_F(DialogSettingsTest, DetailedMessageBoxGeometry)
{
  const MessageEvent message{"title", "text", "informative", "detailed"};
  {
    DetailedMessageBox box("MessageBox", m_manager, message);
    box.resize(321, 234);
    box.done(QDialog::Accepted);
  }

  const std::vector<std::string> expected_keys({"MessageBox/geometry"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);

  const DetailedMessageBox box("MessageBox", m_manager, message);
  EXPECT_EQ(box.size(), QSize(321, 234));
}

TEST_F(DialogSettingsTest, SettingsEditorDialogGeometry)
{
  const PersistentSettingsModel model;
  {
    SettingsEditorDialog dialog(model, m_manager);
    dialog.resize(321, 234);
    dialog.done(QDialog::Rejected);
  }

  const std::vector<std::string> expected_keys({"SettingsEditorDialog/geometry"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);

  const SettingsEditorDialog dialog(model, m_manager);
  EXPECT_EQ(dialog.size(), QSize(321, 234));
}

TEST_F(DialogSettingsTest, CodeViewWritesNothing)
{
  {
    const CodeView view(CodeView::kJSON, m_manager);
  }

  m_manager.Collect(SettingsScope::kApplication);
  EXPECT_TRUE(m_storage.GetKeys().empty());
}

}  // namespace sup::gui::test
