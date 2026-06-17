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

#include "tree_expand_controller.h"

#include <QAbstractItemModel>
#include <QTreeView>
#include <algorithm>

namespace
{

//! Returns the path of an index as a sequence of rows from the root (top-most first).
std::vector<int> GetIndexPath(QModelIndex index)
{
  std::vector<int> path;
  for (; index.isValid(); index = index.parent())
  {
    path.push_back(index.row());
  }
  std::reverse(path.begin(), path.end());
  return path;
}

//! Reconstructs an index in the given model from a row path, or returns an invalid index if the
//! path does not exist in the model.
QModelIndex GetIndexFromPath(const QAbstractItemModel* model, const std::vector<int>& path)
{
  QModelIndex index;  // start from the root
  for (const int row : path)
  {
    index = model->index(row, 0, index);
    if (!index.isValid())
    {
      return {};
    }
  }
  return index;
}

}  // namespace

namespace sup::gui
{

TreeExpandController::TreeExpandController(QObject* parent_object) : QObject(parent_object) {}

TreeExpandController::~TreeExpandController() = default;

void TreeExpandController::AddTreeView(QTreeView* tree_view)
{
  if (!tree_view
      || std::find(m_tree_views.begin(), m_tree_views.end(), tree_view) != m_tree_views.end())
  {
    return;
  }

  m_tree_views.push_back(tree_view);

  connect(tree_view, &QTreeView::expanded, this,
          [this, tree_view](const QModelIndex& index)
          { OnBranchChanged(tree_view, index, true); });
  connect(tree_view, &QTreeView::collapsed, this,
          [this, tree_view](const QModelIndex& index)
          { OnBranchChanged(tree_view, index, false); });

  // Remove from list automatically when the view is destroyed.
  connect(tree_view, &QObject::destroyed, this,
          [this, tree_view]()
          {
            m_tree_views.erase(std::remove(m_tree_views.begin(), m_tree_views.end(), tree_view),
                               m_tree_views.end());
          });
}

void TreeExpandController::RemoveTreeView(QTreeView* tree_view)
{
  auto it = std::find(m_tree_views.begin(), m_tree_views.end(), tree_view);
  if (it == m_tree_views.end())
  {
    return;
  }

  m_tree_views.erase(it);

  disconnect(tree_view, nullptr, this, nullptr);
}

void TreeExpandController::SetActive(bool value)
{
  m_is_active = value;
}

bool TreeExpandController::IsActive() const
{
  return m_is_active;
}

void TreeExpandController::OnBranchChanged(QTreeView* source, const QModelIndex& index,
                                           bool expanded)
{
  if (!m_is_active || m_is_propagating)
  {
    return;
  }

  const auto path = GetIndexPath(index);

  m_is_propagating = true;
  for (auto* tree_view : m_tree_views)
  {
    if (tree_view == source)
    {
      continue;
    }

    if (auto target_index = GetIndexFromPath(tree_view->model(), path); target_index.isValid())
    {
      tree_view->setExpanded(target_index, expanded);
    }
  }
  m_is_propagating = false;
}

}  // namespace sup::gui
