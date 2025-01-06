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

#ifndef SUP_GUI_COMPONENTS_TREE_HELPER_H_
#define SUP_GUI_COMPONENTS_TREE_HELPER_H_

//! @file
//! Helper methods to deal with Qt trees.

#include <functional>

class QMenu;
class QTreeView;
class QPoint;
class QHeaderView;
class QModelIndex;

namespace sup::gui
{

/**
 * @brief Adds collapse/expand section to the QMenu.
 */
void SetupCollapseExpandMenu(const QPoint& point, QMenu& menu, QTreeView& tree_view);

/**
 * @brief Summons collapse/expand menu.
 *
 * @param point Coordinate of context menu as reported by customContextMenuRequested slot.
 * @param tree_vew Tree view which is asking for context menu.
 */
void SummonCollapseExpandMenu(const QPoint& point, QTreeView& tree_view);

/**
 * @brief Creates a callback for given tree, which it can use to process context menu request to
 * from collapse/expand menu.
 */
std::function<void(const QPoint& point)> CreateOnCustomMenuCallback(QTreeView& tree_view);

/**
 * @brief Adjusts column width so they occupy whole horizontal space available for the tree.
 *
 * @param tree The tree to ajust
 * @param stretch_factors Relative stretch factors
 */
void AdjustWidthOfColumns(QHeaderView* header, std::vector<int> stretch_factors);

/**
 * @brief Adjusts column width so they occupy whole horizontal space available for the tree.
 *
 * @param tree The tree to ajust
 * @param stretch_factors Relative stretch factors
 */
void AdjustWidthOfColumns(QTreeView& tree, std::vector<int> stretch_factors);

/**
 * @brief Scrolls tree viewport to selection.
 *
 * @Performs scrolling of the tree viewport to show the current selection. The scrolling will be
 * performed only if the current selection is located outside of the viewport. The viewport will be
 * positioned to show selected item at the top.
 */
void ScrollTreeViewportToSelection(QTreeView& tree_view);

/**
 * @brief Returns index of item which should be highlighted instead of the given item.
 *
 * The algorithm is used in the context of highlighting the current running instruction,
 * when it is hidden inside collapsed branch of its parent. In this case we want to highlight the
 * parent itself.
 *
 * The algorithm will go up in the hierarchy and look for parent, containing our child in
 * one of collapsed branches. If all branches are expanded, will return child back.
 *
 * @param tree The tree with some branches collapsed and some expanded.
 * @param child The index of a child which we would like to highlight.
 *
 * @return The actual index of a parent that we have to highlight instead.
 */
QModelIndex FindVisibleCandidate(const QTreeView& tree, const QModelIndex& child);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_TREE_HELPER_H_
