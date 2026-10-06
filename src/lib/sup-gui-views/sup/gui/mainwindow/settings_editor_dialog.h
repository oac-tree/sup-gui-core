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

#ifndef SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
#define SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_

#include <QDialog>
#include <functional>
#include <memory>

class QLabel;

namespace mvvm
{
class ISessionModel;
}

namespace sup::gui
{

class ISettingsManager;
class SettingsEditor;

/**
 * @brief The SettingsEditorDialog class is a modal dialog that wraps SettingsEditor.
 */
class SettingsEditorDialog : public QDialog
{
  Q_OBJECT

public:
  using widget_factory_func_t = std::function<std::unique_ptr<QWidget>()>;

  /**
   * @brief Main c-tor.
   *
   * The geometry is restored on construction and stored when the dialog is closed. The settings
   * manager must outlive the dialog.
   */
  SettingsEditorDialog(const mvvm::ISessionModel& model, ISettingsManager& settings_manager,
                       QWidget* parent_widget = nullptr);
  ~SettingsEditorDialog() override;

  SettingsEditorDialog(const SettingsEditorDialog&) = delete;
  SettingsEditorDialog& operator=(const SettingsEditorDialog&) = delete;
  SettingsEditorDialog(SettingsEditorDialog&&) = delete;
  SettingsEditorDialog& operator=(SettingsEditorDialog&&) = delete;

  /**
   * @brief Rewrites given model content with local settings copy.
   */
  void PropagateSettingsToModel(mvvm::ISessionModel& model);

  /**
   * @brief Registers widget factory function for the given key.
   */
  void RegisterWidget(const std::string& key, const widget_factory_func_t& factory_func);

  void done(int result) override;

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  QLabel* m_label{nullptr};
  SettingsEditor* m_settings_editor{nullptr};
  ISettingsManager& m_settings_manager;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
