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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_

#include <QModelIndex>
#include <QWidget>

class QTreeView;
class QLineEdit;
class QTimer;

namespace mvvm
{
class ISessionModel;
class SessionItem;
class ItemSelection;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class ISettingsManager;
class SettingsHeaderView;
class TreeViewComponentProvider;

/**
 * @brief The AnyValueEditorTreePanel class represents a panel with a tree and filtering field on
 * the left side of AnyValueEditorWidget.
 */
class AnyValueEditorTreePanel : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Column widths are stored in application settings. The settings manager must outlive the panel.
   */
  explicit AnyValueEditorTreePanel(ISettingsManager& settings_manager,
                                   QWidget* parent_widget = nullptr);
  ~AnyValueEditorTreePanel() override;

  AnyValueEditorTreePanel(const AnyValueEditorTreePanel&) = delete;
  AnyValueEditorTreePanel& operator=(const AnyValueEditorTreePanel&) = delete;
  AnyValueEditorTreePanel(AnyValueEditorTreePanel&&) = delete;
  AnyValueEditorTreePanel& operator=(AnyValueEditorTreePanel&&) = delete;

  /**
   * @brief Sets the container with AnyValueItem.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  std::vector<AnyValueItem*> GetSelectedItems() const;

  void SetSelected(const mvvm::SessionItem* item);

  QTreeView* GetTreeView();

signals:
  void SelectionChanged(const mvvm::ItemSelection&);

private:
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  QLineEdit* m_line_edit{nullptr};
  SettingsHeaderView* m_header{nullptr};

  //! Coalesces bursts of keystrokes in the filter field into a single filter change.
  QTimer* m_filter_timer{nullptr};

  std::unique_ptr<TreeViewComponentProvider> m_component_provider;
  QModelIndex m_last_edited_index;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
