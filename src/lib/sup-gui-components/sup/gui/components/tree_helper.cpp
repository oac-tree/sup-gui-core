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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "tree_helper.h"

#include <QAction>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QTreeView>

namespace sup::gui
{

void SetupCollapseExpandMenu(const QPoint &point, QMenu &menu, QTreeView &tree_view)
{
  menu.setToolTipsVisible(true);

  // expand all
  auto expand_all_action = menu.addAction("Expand all");
  (void)QObject::connect(expand_all_action, &QAction::triggered, &tree_view, &QTreeView::expandAll);

  // expand to depth
  auto expand_to_depth_menu = menu.addMenu("Expand all to depth");
  const size_t max_depth_level = 5;
  for (size_t depth = 0; depth < max_depth_level; ++depth)
  {
    auto action = expand_to_depth_menu->addAction(QString("depth %1").arg(depth + 1));
    auto on_action = [&tree_view, depth]() { tree_view.expandToDepth(depth); };
    (void)QObject::connect(action, &QAction::triggered, &tree_view, on_action);
  }

  // collapse all
  auto collapse_all_action = menu.addAction("Collapse all");
  (void)QObject::connect(collapse_all_action, &QAction::triggered, &tree_view,
                         &QTreeView::collapseAll);

  (void) menu.addSeparator();

  // expand selected
  auto expand_selected_action = menu.addAction("Expand selected");
  auto on_expand_selected = [&tree_view, &point]() { tree_view.expand(tree_view.indexAt(point)); };
  (void)QObject::connect(expand_selected_action, &QAction::triggered, &tree_view,
                         on_expand_selected);

  // expand selected depth

  // FEATURE Current behavior of "Expand all to depth" and "Expand selected to depth" differs for
  // some reason. "Expand all to depth" works correctly in both directions. "Expand selected to
  // depth" doesn't work when a tree is expanded to level 2, and we want to go to level 1, for
  // example. I think it's because of the difference in QTreeView::expandToDepth and
  // QTreeView::expandRecursively which is used below.

  auto expand_selected_to_depth_menu = menu.addMenu("Expand selected to depth");
  for (size_t depth = 0; depth < max_depth_level; ++depth)
  {
    auto action = expand_selected_to_depth_menu->addAction(QString("depth %1").arg(depth + 1));
    auto on_action = [&tree_view, depth, point]()
    { tree_view.expandRecursively(tree_view.indexAt(point), depth); };
    (void)QObject::connect(action, &QAction::triggered, &tree_view, on_action);
  }

  // collapse selected
  auto collapse_selected_action = menu.addAction("Collapse selected");
  auto on_collapse_selected = [&tree_view, point]()
  { tree_view.collapse(tree_view.indexAt(point)); };
  (void)QObject::connect(collapse_selected_action, &QAction::triggered, &tree_view,
                         on_collapse_selected);
}

void SummonCollapseExpandMenu(const QPoint &point, QTreeView &tree_view)
{
  QMenu menu;
  SetupCollapseExpandMenu(point, menu, tree_view);
  (void)menu.exec(tree_view.mapToGlobal(point));
}

// cppcheck-suppress unusedFunction
std::function<void(const QPoint &)> CreateOnCustomMenuCallback(QTreeView &tree_view)
{
  tree_view.setContextMenuPolicy(Qt::CustomContextMenu);
  auto result = [&tree_view](const QPoint &point) { SummonCollapseExpandMenu(point, tree_view); };
  return result;
}

void AdjustWidthOfColumns(QHeaderView *header, std::vector<int> stretch_factors)
{
  // adjust array of stretch factors so it matches number of columns
  const int default_stretch{1};
  stretch_factors.resize(static_cast<std::size_t>(header->count()), default_stretch);
  auto stretch_factor_sum = std::accumulate(stretch_factors.begin(), stretch_factors.end(), 0);

  const auto width = header->width();
  for (int i = 0; i < header->count(); ++i)
  {
    // set column width proportional to stretch factors
    header->resizeSection(
        i, width * stretch_factors[static_cast<std::size_t>(i)] / stretch_factor_sum);
  }

  // last column might be off by one pixel
  header->setStretchLastSection(true);
}

void AdjustWidthOfColumns(QTreeView &tree, std::vector<int> stretch_factors)
{
  AdjustWidthOfColumns(tree.header(), std::move(stretch_factors));
}

// cppcheck-suppress unusedFunction
void ScrollTreeViewportToSelection(QTreeView &tree_view)
{
  auto indexes = tree_view.selectionModel()->selectedIndexes();
  if (indexes.empty())
  {
    return;
  }

  auto visible_rectangle = tree_view.visualRect(indexes.front());
  if (visible_rectangle.top() < 0 || visible_rectangle.bottom() > tree_view.rect().bottom())
  {
    tree_view.scrollTo(indexes.front(), QAbstractItemView::PositionAtTop);
  }
}

// cppcheck-suppress unusedFunction
QModelIndex FindVisibleCandidate(const QTreeView &tree, const QModelIndex &child)
{
  if (!child.isValid())
  {
    return {};
  }
  QModelIndex result = child;

  QModelIndex current = child;
  while (current.isValid())
  {
    const int row_count = tree.model()->rowCount(current);
    if (!tree.isExpanded(current) && (row_count > 0))
    {
      result = current;
    }

    current = current.parent();
  }

  return result;
}

}  // namespace sup::gui
