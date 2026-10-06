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

#include "sup/gui/widgets/settings_header_view.h"

#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

#include <gtest/gtest.h>

#include <QStandardItemModel>
#include <QTreeView>
#include <memory>

namespace sup::gui::test
{

namespace
{

constexpr auto kGroup = "View/header";

}  // namespace

/**
 * @brief Tests for SettingsHeaderView class.
 */
class SettingsHeaderViewTest : public ::testing::Test
{
public:
  SettingsHeaderViewTest() : m_model(1, 2)
  {
    m_manager.SetStorage(SettingsScope::kApplication, &m_storage);
  }

  //! Creates a tree with the header of the given group.
  std::unique_ptr<QTreeView> CreateTree(SettingsHeaderView*& header)
  {
    auto result = std::make_unique<QTreeView>();
    header = new SettingsHeaderView(kGroup, m_manager, {1, 1});
    result->setHeader(header);
    result->setModel(&m_model);
    return result;
  }

  //! Returns the state stored in the manager.
  QByteArray GetStoredState() const
  {
    return m_manager.Get<HeaderInfo>(kGroup, SettingsScope::kApplication).state;
  }

  QStandardItemModel m_model;
  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
};

TEST_F(SettingsHeaderViewTest, InitialState)
{
  SettingsHeaderView* header{nullptr};
  auto tree = CreateTree(header);

  EXPECT_TRUE(header->GetHeaderInfo().state.isEmpty());
}

TEST_F(SettingsHeaderViewTest, StoredStateIsRestored)
{
  SettingsHeaderView* header0{nullptr};
  auto tree0 = CreateTree(header0);
  header0->resizeSection(0, 123);
  m_manager.Set(kGroup, HeaderInfo{header0->saveState()}, SettingsScope::kApplication);

  SettingsHeaderView* header1{nullptr};
  auto tree1 = CreateTree(header1);
  header1->AdjustColumnsWidth();

  EXPECT_EQ(header1->sectionSize(0), 123);
}

TEST_F(SettingsHeaderViewTest, NotAdjustedHeaderProvidesStoredState)
{
  const QByteArray stored_state("state");
  m_manager.Set(kGroup, HeaderInfo{stored_state}, SettingsScope::kApplication);

  SettingsHeaderView* header{nullptr};
  auto tree = CreateTree(header);

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_EQ(header->GetHeaderInfo().state, stored_state);
  EXPECT_EQ(GetStoredState(), stored_state);
}

TEST_F(SettingsHeaderViewTest, NotAdjustedHeaderDoesNotOverwriteAdjustedOne)
{
  m_manager.Set(kGroup, HeaderInfo{QByteArray("state")}, SettingsScope::kApplication);

  SettingsHeaderView* header0{nullptr};
  auto tree0 = CreateTree(header0);
  SettingsHeaderView* header1{nullptr};
  auto tree1 = CreateTree(header1);

  // reset counts as user adjustment, the user state becomes empty
  header0->ResetColumnWidth();

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_TRUE(GetStoredState().isEmpty());
}

TEST_F(SettingsHeaderViewTest, DestroyedHeaderIsNotCollected)
{
  {
    SettingsHeaderView* header{nullptr};
    auto tree = CreateTree(header);
    header->ResetColumnWidth();
  }

  const QByteArray stored_state("state");
  m_manager.Set(kGroup, HeaderInfo{stored_state}, SettingsScope::kApplication);
  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_EQ(GetStoredState(), stored_state);
}

}  // namespace sup::gui::test
