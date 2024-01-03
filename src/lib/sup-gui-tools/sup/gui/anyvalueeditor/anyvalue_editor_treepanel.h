/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_

#include <QWidget>

class QTreeView;
class QLineEdit;

namespace mvvm
{
class ApplicationModel;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class CustomHeaderView;
class TreeViewComponentProvider;

//! A panel with a tree on the left side of AnyValueEditor.

class AnyValueEditorTreePanel : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorTreePanel(mvvm::ApplicationModel* model, QWidget* parent = nullptr);
  ~AnyValueEditorTreePanel() override;

  sup::gui::AnyValueItem* GetSelectedItem() const;

  void SetSelected(mvvm::SessionItem* item);

  QTreeView* GetTreeView();

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  QLineEdit* m_line_edit{nullptr};
  CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<TreeViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
