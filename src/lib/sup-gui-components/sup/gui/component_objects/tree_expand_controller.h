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

#ifndef SUP_GUI_COMPONENT_OBJECTS_TREE_EXPAND_CONTROLLER_H_
#define SUP_GUI_COMPONENT_OBJECTS_TREE_EXPAND_CONTROLLER_H_

#include <QObject>
#include <vector>

class QTreeView;
class QModelIndex;

namespace sup::gui
{

/**
 * @brief The TreeExpandController class synchronises the expand/collapse state of branches across
 * multiple QTreeView instances.
 *
 * After registering two or more tree views via AddTreeView(), expanding or collapsing a branch in
 * one view is immediately mirrored to all other registered views. Synchronisation can be
 * temporarily suspended with SetActive(false).
 *
 * All registered trees are expected to share the same tree structure (only the data may differ).
 * A branch is identified by its row path from the root, which is reconstructed against each
 * target view's own model.
 */
class TreeExpandController : public QObject
{
  Q_OBJECT

public:
  explicit TreeExpandController(QObject* parent_object = nullptr);
  ~TreeExpandController() override;

  TreeExpandController(const TreeExpandController&) = delete;
  TreeExpandController& operator=(const TreeExpandController&) = delete;
  TreeExpandController(TreeExpandController&&) = delete;
  TreeExpandController& operator=(TreeExpandController&&) = delete;

  /**
   * @brief Adds a tree view to the synchronisation group.
   *
   * Does nothing if @p tree_view is nullptr or already registered.
   */
  void AddTreeView(QTreeView* tree_view);

  /**
   * @brief Removes a tree view from the synchronisation group.
   *
   * Does nothing if @p tree_view is nullptr or not registered.
   */
  void RemoveTreeView(QTreeView* tree_view);

  /**
   * @brief Enables or disables expand/collapse synchronisation without unregistering views.
   */
  void SetActive(bool value);

  /**
   * @brief Returns true when synchronisation is active.
   */
  bool IsActive() const;

private:
  void OnBranchChanged(QTreeView* source, const QModelIndex& index, bool expanded);

  std::vector<QTreeView*> m_tree_views;
  bool m_is_active{true};
  bool m_is_propagating{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENT_OBJECTS_TREE_EXPAND_CONTROLLER_H_
