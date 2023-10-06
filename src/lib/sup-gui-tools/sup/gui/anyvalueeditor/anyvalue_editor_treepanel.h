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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_

#include <QWidget>

class QTreeView;

namespace mvvm
{
class ApplicationModel;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class CustomHeaderView;

//! A panel with a tree on the left side of AnyValueEditor.

class AnyValueEditorTreePanel : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorTreePanel(mvvm::ApplicationModel* model, QWidget* parent = nullptr);
  ~AnyValueEditorTreePanel() override;

  sup::gui::AnyValueItem* GetSelectedItem() const;

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
