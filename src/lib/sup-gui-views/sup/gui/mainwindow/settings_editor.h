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

#ifndef SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_H_
#define SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_H_

#include <QWidget>
#include <memory>

class QSplitter;
class QListView;
class QStackedWidget;

namespace mvvm
{
class ViewModel;
class ItemViewComponentProvider;
class SessionItem;
class ISessionModel;
}  // namespace mvvm

namespace sup::gui
{

class CommonSettingsEditor;
class StackedWidgetProvider;

/**
 * @brief The SettingsEditor class represents a simple editor of application persistent settings.
 *
 * It consists of the list with group setting items on the left, and a property editor for
 * the currently selected group on the right.
 */
class SettingsEditor : public QWidget
{
  Q_OBJECT

public:
  using widget_factory_func_t = std::function<std::unique_ptr<QWidget>()>;

  explicit SettingsEditor(const mvvm::ISessionModel& model, QWidget* parent_widget = nullptr);
  ~SettingsEditor() override;

  SettingsEditor(const SettingsEditor&) = delete;
  SettingsEditor& operator=(const SettingsEditor&) = delete;
  SettingsEditor(SettingsEditor&&) = delete;
  SettingsEditor& operator=(SettingsEditor&&) = delete;

  /**
   * @brief Sets settings group item to show in property editor.
   */
  void SetSettingsGroup(mvvm::SessionItem* item);

  /**
   * @brief Rewrites given model content with local settings copy.
   */
  void PropagateSettingsToModel(mvvm::ISessionModel& model);

  /**
   * @brief Registers widget factory function for the given key.
   */
  void RegisterWidget(const std::string& key, const widget_factory_func_t& factory_func);

private:
  /**
   * @brief Sets initial values for editing.
   */
  void SetInitialValues(const mvvm::ISessionModel& model);

  QSplitter* m_splitter{nullptr};
  QListView* m_list_view{nullptr};
  QStackedWidget* m_stacked_widget{nullptr};
  CommonSettingsEditor* m_common_setting_editor{nullptr};

  std::unique_ptr<mvvm::ISessionModel> m_working_settings_copy;
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_list_component_provider;
  std::unique_ptr<StackedWidgetProvider> m_stacked_widget_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_H_
