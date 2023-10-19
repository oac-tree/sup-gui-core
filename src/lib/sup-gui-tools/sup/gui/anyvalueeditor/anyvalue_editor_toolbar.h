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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TOOLBAR_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TOOLBAR_H_

#include <QToolBar>
#include <memory>

class QToolButton;
class QMenu;

namespace sup::gui
{

class AnyValueEditorActionHandler;

//! A toolbar on top of AnyValueEditor.

class AnyValueEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit AnyValueEditorToolBar(AnyValueEditorActionHandler* actions, QWidget* parent = nullptr);
  ~AnyValueEditorToolBar() override;

signals:
  void ToggleTextPanelVisibilityRequest();
  void ImportFromFileRequest();
  void ExportToFileRequest();
  void MakeJSONPrettyRequest(bool);

private:
  void InsertStrech();
  std::unique_ptr<QMenu> CreateAddAnyValueMenu();
  std::unique_ptr<QMenu> CreateSettingsMenu();

  QToolButton* m_add_anyvalue_button{nullptr};
  QToolButton* m_remove_button{nullptr};
  QToolButton* m_export_button{nullptr};
  QToolButton* m_hide_pannel_button{nullptr};
  QToolButton* m_details_button{nullptr};

  AnyValueEditorActionHandler* m_actions{nullptr};

  std::unique_ptr<QMenu> m_create_anyvalue_menu;
  std::unique_ptr<QMenu> m_add_field_menu;
  std::unique_ptr<QMenu> m_settings_menu;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TOOLBAR_H_
