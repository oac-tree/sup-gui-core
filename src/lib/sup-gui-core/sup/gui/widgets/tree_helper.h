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

#ifndef SUP_GUI_WIDGETS_TREE_HELPER_H_
#define SUP_GUI_WIDGETS_TREE_HELPER_H_

//! Helper methods to deal with Qt trees.

#include <functional>

class QMenu;
class QTreeView;
class QPoint;

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

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_TREE_HELPER_H_
