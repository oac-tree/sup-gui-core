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

#ifndef SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_
#define SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_

#include <QObject>

class QAction;

namespace sup::gui
{

class AppCommandManager;
class AppContextManager;

/**
 * @brief The AppContextFocusController class handles the context switch on the focus widget
 * change.
 *
 * It shall be connected with the main window and listen for focusChanged signals. It uses
 * AppContextManager to find a list of active contexts, and AppCommandManager manager to
 * enable/disable all registered commands according to the contexts.
 */
class AppContextFocusController : public QObject
{
  Q_OBJECT

public:
  explicit AppContextFocusController(AppContextManager& context_manager,
                                     AppCommandManager& command_manager, QObject* parent_object);
  ~AppContextFocusController() override;

  void OnFocusWidgetUpdate(QWidget* old, QWidget* now);

private:
  AppContextManager& m_context_manager;
  AppCommandManager& m_command_manager;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_FOCUS_CONTROLLER_H_
