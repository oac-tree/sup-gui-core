/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "tree_view_component_provider.h"

#include <sup/gui/viewmodel/anyvalue_filtered_viewmodel.h>
#include <sup/gui/viewmodel/anyvalue_viewmodel.h>

#include <mvvm/delegates/viewmodel_delegate.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>

#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QTreeView>

namespace sup::gui
{

TreeViewComponentProvider::TreeViewComponentProvider(mvvm::ApplicationModel *model, QTreeView *view)
    : m_view_model(std::make_unique<AnyValueViewModel>(model))
    , m_proxy_model(std::make_unique<AnyValueFilteredViewModel>())
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
    , m_tree_view(view)
{
  m_proxy_model->setSourceModel(m_view_model.get());
  m_proxy_model->setRecursiveFilteringEnabled(true);

  m_tree_view->setModel(m_proxy_model.get());
  m_tree_view->setItemDelegate(m_delegate.get());
}

TreeViewComponentProvider::~TreeViewComponentProvider() = default;

void TreeViewComponentProvider::SetFilterPattern(const QString &pattern)
{
  m_proxy_model->SetPattern(pattern);
}

const mvvm::SessionItem *TreeViewComponentProvider::GetSelectedItem() const
{
  auto selected = GetSelectedItems();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<const mvvm::SessionItem *> TreeViewComponentProvider::GetSelectedItems() const
{
  std::vector<const mvvm::SessionItem *> result;

  for (auto index : GetSelectionModel()->selectedIndexes())
  {
    // skipping nullptr
    if (auto item = GetItemFromViewIndex(index); item)
    {
      result.push_back(item);
    }
  }

  return mvvm::utils::UniqueWithOrder(result);
}

void TreeViewComponentProvider::SetSelectedItem(const mvvm::SessionItem *item)
{
  SetSelectedItems({item});
}

void TreeViewComponentProvider::SetSelectedItems(
    const std::vector<const mvvm::SessionItem *> &items)
{
  GetSelectionModel()->clearSelection();
  QItemSelection selection;
  for (auto item : items)
  {
    for (auto index : GetViewIndices(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }

  //  auto flags = QItemSelectionModel::Select;  // not clear, which one to use
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;
  GetSelectionModel()->select(selection, flags);
}

QItemSelectionModel *TreeViewComponentProvider::GetSelectionModel() const
{
  return m_tree_view->selectionModel();
}

const mvvm::ViewModel *TreeViewComponentProvider::GetViewModel() const
{
  return m_view_model.get();
}

const mvvm::SessionItem *TreeViewComponentProvider::GetItemFromViewIndex(
    const QModelIndex &index) const
{
  auto source_index = m_proxy_model ? m_proxy_model->mapToSource(index) : index;
  return GetViewModel()->GetSessionItemFromIndex(source_index);
}

QList<QModelIndex> TreeViewComponentProvider::GetViewIndices(const mvvm::SessionItem *item) const
{
  const auto source_indices = GetViewModel()->GetIndexOfSessionItem(item);

  if (!m_proxy_model)
  {
    return source_indices;
  }

  auto on_index = [this](auto index) { return m_proxy_model->mapFromSource(index); };
  QList<QModelIndex> result;
  std::transform(std::begin(source_indices), std::end(source_indices), std::back_inserter(result),
                 on_index);

  return result;
}

}  // namespace sup::gui
