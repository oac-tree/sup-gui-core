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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_

#include <sup/gui/components/anyvalue_editor_context.h>

#include <mvvm/model/i_session_model.h>

#include <QString>
#include <QWidget>

#include <memory>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class AnyValueEditorActionHandler;
class AnyValueEditorTextPanel;
class AnyValueEditorTreePanel;
class AnyValueEditorActions;
class CustomSplitter;

/**
 * @brief The AnyValueEditorWidget class is a main widget of AnyValueEditor.
 *
 * It contains a toolbar, a tree for AnyValueItem editing, and optional panel with JSON
 * representation.
 */
class AnyValueEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorWidget(QWidget* parent_widget = nullptr);
  ~AnyValueEditorWidget() override;

  /**
   * @brief Sets the container with AnyValueItem.
   *
   * This editor is build around the idea, that there is only a single AnyValueItem in a container.
   * It can be either root item of the model, mvvm::ContainerItem or any other container-like item.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  /**
   * @brief Provides import of AnyValue from JSON file.
   */
  void OnImportFromFileRequest();

  /**
   * @brief Exports top-level AnyValue to JSON file.
   */
  void OnExportToFileRequest();

  /**
   * @brief Import waveform from external dialog.
   */
  void OnImportWaveformRequest();

  /**
   * @brief Sets initial value.
   *
   * The given value will be cloned inside the editor's model and used as a starting point for
   * editing.
   */
  void SetInitialValue(const AnyValueItem& item);

  /**
   * @brief Returns top AnyValueItem, which is our result.
   */
  AnyValueItem* GetTopItem();

  /**
   * @brief Returns the result of editing.
   */
  std::unique_ptr<AnyValueItem> GetResult();

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void SetupWidgetActions();

  /**
   * @brief Imports AnyValue from JSON file.
   */
  void ImportAnyValueFromFile(const QString& file_name);

  /**
   * @brief Creates a context with all callbacks necessary for AnyValueEditorActions to function.
   */
  AnyValueEditorContext CreateActionContext() const;

  /**
   * @brief Updates current working dir to use next time for input.
   */
  void UpdateCurrentWorkdir(const QString& file_name);

  /**
   * @brief Creates context menu for AnyValueItem tree.
   */
  void OnContextMenuRequest(const QPoint& point);

  QWidget* CreateLeftPanel();
  QWidget* CreateRightPanel();

  QAction* m_show_right_sidebar{nullptr};

  std::unique_ptr<AnyValueEditorActionHandler> m_action_handler;
  AnyValueEditorActions* m_actions{nullptr};
  AnyValueEditorTextPanel* m_text_panel{nullptr};
  AnyValueEditorTreePanel* m_tree_panel{nullptr};
  QWidget* m_left_panel{nullptr};
  QWidget* m_right_panel{nullptr};
  CustomSplitter* m_splitter{nullptr};

  QString m_current_workdir;  //! directory used during import/export operations
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_WIDGET_H_
