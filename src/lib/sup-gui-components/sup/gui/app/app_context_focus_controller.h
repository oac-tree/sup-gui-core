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

#ifndef SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_
#define SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_

#include <QPointer>
#include <QWidget>

namespace sup::gui
{

class IAppCommandRegistry;
class IAppCommandContextWidgetMap;

/**
 * @brief The AppContextFocusController class handles the context switch on the focus widget
 * change.
 *
 * It shall be connected with the main window and listen for focusChanged signals. It uses
 * AppContextManager to find a list of active contexts, and AppCommandManager manager to
 * enable/disable all registered commands according to the contexts.
 */
class AppContextFocusController
{
public:
  explicit AppContextFocusController(IAppCommandContextWidgetMap& widget_map,
                                     IAppCommandRegistry& command_registry);
  ~AppContextFocusController() = default;

  AppContextFocusController(const AppContextFocusController&) = delete;
  AppContextFocusController& operator=(const AppContextFocusController&) = delete;
  AppContextFocusController(AppContextFocusController&&) = delete;
  AppContextFocusController& operator=(AppContextFocusController&&) = delete;

  /**
   * @brief Sets the context stack of all commands from the new focus widget and its parents.
   *
   * @details A nested call, made while the commands are updated, doesn't apply its widget
   * directly. The running call applies the latest focus widget before it returns.
   */
  void OnFocusWidgetUpdate(const QWidget* old, QWidget* now);

private:
  IAppCommandContextWidgetMap& m_widget_map;
  IAppCommandRegistry& m_command_registry;
  QPointer<QWidget> m_focus_widget;  //!< latest focus widget
  bool m_is_updating{false};         //!< true while the commands are updated
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_
