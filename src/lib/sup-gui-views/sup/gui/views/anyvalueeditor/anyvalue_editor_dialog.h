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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_

#include <QDialog>
#include <memory>
#include <string>

namespace sup::gui
{

class AbstractAnyValueEditor;
class AnyValueItem;
class ISettingsManager;

/**
 * @brief The AnyValueEditorDialog class is a modal dialog containing one of AnyValue editors.
 */
class AnyValueEditorDialog : public QDialog
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The dialog geometry is restored on construction and stored when the dialog is closed. The
   * settings manager must outlive the dialog.
   */
  AnyValueEditorDialog(std::unique_ptr<AbstractAnyValueEditor> editor,
                       ISettingsManager& settings_manager, QWidget* parent_widget = nullptr);
  ~AnyValueEditorDialog() override;

  AnyValueEditorDialog(const AnyValueEditorDialog&) = delete;
  AnyValueEditorDialog& operator=(const AnyValueEditorDialog&) = delete;
  AnyValueEditorDialog(AnyValueEditorDialog&&) = delete;
  AnyValueEditorDialog& operator=(AnyValueEditorDialog&&) = delete;

  /**
   * @brief Sets initial value to the editor.
   */
  void SetInitialValue(const AnyValueItem* item);

  /**
   * @brief Returns result of the editing.
   *
   * It is supposed to be used after the dialog close.
   */
  std::unique_ptr<AnyValueItem> GetResult();

  void SetDescription(const QString& description);

  void done(int result) override;

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  //! Returns the settings group of the dialog geometry.
  std::string GetSettingsGroup() const;

  AbstractAnyValueEditor* m_anyvalue_editor{nullptr};
  ISettingsManager& m_settings_manager;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_
