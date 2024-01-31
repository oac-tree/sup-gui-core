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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>

#include <QString>
#include <QWidget>
#include <memory>

class QSplitter;

namespace mvvm
{
class ApplicationModel;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class AnyValueEditorActionHandler;
class AnyValueEditorTextPanel;
class AnyValueEditorTreePanel;
class AnyValueEditorActions;

/**
 * @brief The AnyValueEditorWidget class is a main widget of AnyValueEditor.
 *
 * @details It contains a toolbar, a tree for AnyValueItem editing, and optional panel with JSON
 * representation.
 */
class AnyValueEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorWidget(mvvm::ApplicationModel* model, QWidget* parent = nullptr);
  ~AnyValueEditorWidget() override;

  /**
   * @brief Sets the container with AnyValueItem.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  void OnImportFromFileRequest();
  void OnExportToFileRequest();

  sup::gui::AnyValueItem* GetSelectedItem() const;

  void SetInitialValue(const sup::gui::AnyValueItem& item);

  AnyValueItem* GetTopItem();

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void SetupWidgetActions();
  void ImportAnyValueFromFile(const QString& file_name);
  AnyValueEditorContext CreateActionContext() const;
  void UpdateCurrentWorkdir(const QString& file_name);

  QWidget* CreateLeftPanel();
  QWidget* CreateRightPanel();

  QAction* m_show_right_sidebar{nullptr};

  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueEditorActions* m_actions{nullptr};
  AnyValueEditorActionHandler* m_action_handler{nullptr};
  AnyValueEditorTextPanel* m_text_edit{nullptr};
  AnyValueEditorTreePanel* m_tree_panel{nullptr};
  QWidget* m_left_panel{nullptr};
  QWidget* m_right_panel{nullptr};
  QSplitter* m_splitter{nullptr};

  QString m_current_workdir;  //! directory used during import/export operations
  bool m_text_panel_is_visible{true};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_
