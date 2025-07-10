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

#ifndef SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
#define SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_

#include <QDialog>

class QLabel;

namespace mvvm
{
class ISessionModel;
}

namespace sup::gui
{

class SettingsEditor;
class SettingsModel;

/**
 * @brief The SettingsEditorDialog class is a modal dialog that wraps SettingsEditor.
 */
class SettingsEditorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsEditorDialog(QWidget* parent_widget = nullptr);
  ~SettingsEditorDialog() override;

  /**
   * @brief Sets initial values for editing.
   */
  void SetInitialValues(const mvvm::ISessionModel& model);

  /**
   * @brief Returns the model representing result of editing.
   */
  const SettingsModel* GetResult() const;

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  /**
   * @brief Saves results of editing in persistent settings of QSettings file.
   */
  void SaveToPersistentSettings();

  /**
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings();

  QLabel* m_label{nullptr};
  SettingsEditor* m_settings_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
