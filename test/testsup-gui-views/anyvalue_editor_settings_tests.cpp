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

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_dialog.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_treepanel.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>
#include <sup/gui/widgets/settings_header_view.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <gtest/gtest.h>

#include <QTreeView>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValue editor widgets storing settings in the settings manager.
 */
class AnyValueEditorSettingsTest : public ::testing::Test
{
public:
  AnyValueEditorSettingsTest() { m_manager.SetStorage(SettingsScope::kApplication, &m_storage); }

  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
  NullCommandService m_command_service;
};

TEST_F(AnyValueEditorSettingsTest, TreePanelHeader)
{
  AnyValueEditorTreePanel panel(m_manager);

  EXPECT_NE(dynamic_cast<SettingsHeaderView*>(panel.GetTreeView()->header()), nullptr);
}

TEST_F(AnyValueEditorSettingsTest, WidgetCollect)
{
  const AnyValueEditorWidget widget(AppViewContext{m_command_service, m_manager});
  EXPECT_NE(widget.findChild<SettingsSplitter*>(), nullptr);

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_TRUE(m_storage.Read("AnyValueEditorWidget/splitter/state").has_value());
  EXPECT_TRUE(m_storage.Read("AnyValueEditorTreePanel/tree_header/state").has_value());
}

TEST_F(AnyValueEditorSettingsTest, DialogGeometryRoundTrip)
{
  {
    AnyValueEditorDialog dialog(std::make_unique<AnyValueEditor>(), m_manager);
    dialog.resize(321, 234);
    dialog.done(QDialog::Accepted);
  }

  // geometry is stored on close
  EXPECT_EQ(m_storage.GetKeys().size(), 1);

  const AnyValueEditorDialog dialog(std::make_unique<AnyValueEditor>(), m_manager);
  EXPECT_EQ(dialog.size(), QSize(321, 234));
}

TEST_F(AnyValueEditorSettingsTest, AnyValueEditorWithSettingsManager)
{
  const AnyValueEditor editor(m_manager);
  EXPECT_NE(editor.findChild<SettingsSplitter*>(), nullptr);

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_TRUE(m_storage.Read("AnyValueEditorWidget/splitter/state").has_value());
}

}  // namespace sup::gui::test
