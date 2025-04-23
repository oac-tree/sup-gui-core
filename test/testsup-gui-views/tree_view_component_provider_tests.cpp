/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "sup/gui/views/anyvalueeditor/tree_view_component_provider.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/editors/allint_spinbox_editor.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTreeView>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::SessionItem*)
#endif

namespace sup::gui::test
{

//! Testing TreeViewComponentProvider class.

class TreeViewComponentProviderTests : public ::testing::Test
{
public:
  QTreeView m_tree;
  mvvm::ApplicationModel m_model;
};

TEST_F(TreeViewComponentProviderTests, InitialState)
{
  TreeViewComponentProvider provider(&m_model, &m_tree);

  EXPECT_NE(provider.GetViewModel(), nullptr);
  EXPECT_NE(provider.GetLastProxyModel(), nullptr);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());

  ASSERT_NE(dynamic_cast<mvvm::FilterNameViewModel*>(m_tree.model()), nullptr);
  EXPECT_EQ(provider.GetLastProxyModel(), m_tree.model());
}

//! Testing how tree view looks for a single scalar. No filter pattern is set.
TEST_F(TreeViewComponentProviderTests, Scalar)
{
  // single scalar in a model
  auto item = m_model.InsertItem<AnyValueScalarItem>();
  item->SetAnyTypeName(sup::dto::kInt8TypeName);
  item->SetData(mvvm::int8{42});

  TreeViewComponentProvider provider(&m_model, &m_tree);

  QSignalSpy spy_selected(&provider, &TreeViewComponentProvider::SelectedItemChanged);

  // to test tree view we will be looking at proxy model.
  auto proxymodel = provider.GetLastProxyModel();

  // no item is selected
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);

  // check if we can get back to item using visible indices
  auto item_displayname_index = proxymodel->index(0, 0);
  auto item_value_index = proxymodel->index(0, 1);
  auto item_type_index = proxymodel->index(0, 2);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_displayname_index), item);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_value_index), item);

  // check that proxymodel sees correct data
  EXPECT_EQ(proxymodel->data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(proxymodel->data(item_value_index, Qt::DisplayRole).toInt(), 42);

  // making item selected
  provider.SetSelectedItem(item);
  EXPECT_EQ(provider.GetSelectedItem(), item);
  EXPECT_EQ(provider.GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));

  EXPECT_EQ(spy_selected.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), item);
}

//! Testing how tree view looks for a single scalar. The difference with previous test is that
//! AnyValueItem is located in its own container.
TEST_F(TreeViewComponentProviderTests, ScalarInContainer)
{
  TreeViewComponentProvider provider(&m_model, &m_tree);

  // single scalar in a model
  auto container = m_model.InsertItem<mvvm::ContainerItem>();
  auto item = m_model.InsertItem<AnyValueScalarItem>(container, mvvm::TagIndex::Append());
  item->SetAnyTypeName(sup::dto::kInt8TypeName);
  item->SetData(mvvm::int8{42});

  // to test tree view we will be looking at proxy model.
  auto proxymodel = provider.GetLastProxyModel();

  EXPECT_EQ(proxymodel->rowCount(), 0);
  EXPECT_EQ(proxymodel->columnCount(), 3);

  provider.SetItem(container);

  EXPECT_EQ(proxymodel->rowCount(), 1);
  EXPECT_EQ(proxymodel->columnCount(), 3);

  // check if we can get back to item using visible indices
  auto item_displayname_index = proxymodel->index(0, 0);
  auto item_value_index = proxymodel->index(0, 1);
  auto item_type_index = proxymodel->index(0, 2);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_displayname_index), item);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_value_index), item);

  // check that proxymodel sees correct data
  EXPECT_EQ(proxymodel->data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(proxymodel->data(item_value_index, Qt::DisplayRole).toInt(), 42);

  // making item selected
  provider.SetSelectedItem(item);
  EXPECT_EQ(provider.GetSelectedItem(), item);
  EXPECT_EQ(provider.GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));
}

//! Testing how struct looks like with two children, one filtered out
TEST_F(TreeViewComponentProviderTests, FilteredStruct)
{
  auto struct_item = m_model.InsertItem<AnyValueStructItem>();
  auto scalar_item0 = struct_item->AddScalarField("scalar", sup::dto::kInt32TypeName, 42);
  auto scalar_item1 = struct_item->AddScalarField("abc", sup::dto::kInt32TypeName, 43);

  TreeViewComponentProvider provider(&m_model, &m_tree);

  // to test tree view we will be looking at proxy model.
  auto proxymodel = provider.GetLastProxyModel();

  // we see both children beneath
  auto struct_index = proxymodel->index(0, 0);
  EXPECT_EQ(proxymodel->rowCount(struct_index), 2);
  EXPECT_EQ(proxymodel->columnCount(struct_index), 3);

  // filtering out a child with "scalar" display name
  provider.SetFilterPattern("ab");
  EXPECT_EQ(proxymodel->rowCount(struct_index), 1);
  EXPECT_EQ(proxymodel->columnCount(struct_index), 3);

  // our indices at row=0 are actually looking to scalar_item1
  auto scalar_displayname_index = proxymodel->index(0, 0, struct_index);
  auto scalar_value_index = proxymodel->index(0, 1, struct_index);

  EXPECT_EQ(provider.GetItemFromViewIndex(scalar_displayname_index), scalar_item1);
  EXPECT_EQ(provider.GetItemFromViewIndex(scalar_value_index), scalar_item1);

  EXPECT_EQ(proxymodel->data(scalar_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  EXPECT_EQ(proxymodel->data(scalar_value_index, Qt::DisplayRole).toInt(), 43);

  // filtering out everything
  provider.SetFilterPattern("non-existing-pattern");

  // the model is empty
  EXPECT_EQ(proxymodel->rowCount(), 0);
  EXPECT_EQ(proxymodel->columnCount(), 3);
}

//! Testing component provider after model reset.
TEST_F(TreeViewComponentProviderTests, ScalarInContainerAfterModelReset)
{
  TreeViewComponentProvider provider(&m_model, &m_tree);

  // single scalar in a model
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  // to test tree view we will be looking at proxy model.
  auto proxymodel = provider.GetLastProxyModel();

  EXPECT_EQ(proxymodel->rowCount(), 0);
  EXPECT_EQ(proxymodel->columnCount(), 3);

  provider.SetItem(container);

  EXPECT_EQ(proxymodel->rowCount(), 0);
  EXPECT_EQ(proxymodel->columnCount(), 3);

  auto new_root = mvvm::utils::CreateEmptyRootItem();
  auto new_container = new_root->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());
  auto item = m_model.InsertItem<AnyValueScalarItem>(new_container, mvvm::TagIndex::Append());
  item->SetAnyTypeName(sup::dto::kInt8TypeName);
  item->SetData(mvvm::int8{42});

  // mimicking model load from disk
  m_model.ReplaceRootItem(std::move(new_root));
  provider.SetItem(new_container);

  EXPECT_EQ(proxymodel->rowCount(), 1);
  EXPECT_EQ(proxymodel->columnCount(), 3);

  // check if we can get back to item using visible indices
  auto item_displayname_index = proxymodel->index(0, 0);
  auto item_value_index = proxymodel->index(0, 1);
  auto item_type_index = proxymodel->index(0, 2);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_displayname_index), item);
  EXPECT_EQ(provider.GetItemFromViewIndex(item_value_index), item);

  // check that proxymodel sees correct data
  EXPECT_EQ(proxymodel->data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(proxymodel->data(item_value_index, Qt::DisplayRole).toInt(), 42);
}

//! Testing if cell editor is created with correct limits after struct is filtered
TEST_F(TreeViewComponentProviderTests, CheckCellEditorInFilteredStruct)
{
  auto struct_item = m_model.InsertItem<AnyValueStructItem>();
  auto scalar_item0 = struct_item->AddScalarField("scalar", sup::dto::kInt32TypeName, 42);
  auto scalar_item1 = struct_item->AddScalarField("abc", sup::dto::kInt8TypeName, mvvm::int8{43});

  TreeViewComponentProvider provider(&m_model, &m_tree);

  // to test tree view we will be looking at proxy model.
  auto proxymodel = provider.GetLastProxyModel();

  // we see both children beneath
  auto struct_index = proxymodel->index(0, 0);

  // filtering out a child with "scalar" display name
  provider.SetFilterPattern("ab");
  EXPECT_EQ(proxymodel->rowCount(struct_index), 1);
  EXPECT_EQ(proxymodel->columnCount(struct_index), 3);

  // our indices at row=0 are actually looking to scalar_item1
  auto scalar_displayname_index = proxymodel->index(0, 0, struct_index);
  auto scalar_value_index = proxymodel->index(0, 1, struct_index);

  EXPECT_EQ(provider.GetItemFromViewIndex(scalar_displayname_index), scalar_item1);
  EXPECT_EQ(provider.GetItemFromViewIndex(scalar_value_index), scalar_item1);

  // pretending we are editing the value
  auto delegate = m_tree.itemDelegate();
  auto editor = std::unique_ptr<QWidget>(delegate->createEditor(nullptr, {}, scalar_value_index));
  delegate->setEditorData(editor.get(), scalar_value_index);

  // cell editor should get limits as in "abc" scalar field
  auto int_editor = dynamic_cast<mvvm::AllIntSpinBoxEditor*>(editor.get());
  ASSERT_NE(int_editor, nullptr);
  const auto expected_min = std::numeric_limits<mvvm::int8>::lowest();
  const auto expected_max = std::numeric_limits<mvvm::int8>::max();
  EXPECT_EQ(int_editor->GetLowerLimit(), mvvm::variant_t(expected_min));
}

}  // namespace sup::gui::test
