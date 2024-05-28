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

#ifndef SUP_GUI_APP_APP_FOCUS_WIDGET_MANAGER_H_
#define SUP_GUI_APP_APP_FOCUS_WIDGET_MANAGER_H_

#include <QObject>

class QAction;

namespace sup::gui
{

class AppCommandManager;

/**
 * @brief The AppFocusWidgetManager class handles context switch on focus widget change.
 *
 * It shall be connected with the main window and listen for focusChanged signals.
 * It uses AppCommandManager manager to enable/disable all registered commands.
 */
class AppFocusWidgetManager : public QObject
{
  Q_OBJECT

public:
  explicit AppFocusWidgetManager(AppCommandManager& command_manager, QObject* parent);
  ~AppFocusWidgetManager() override;

  void UpdateFocusWidget(QWidget *old, QWidget *now);

private:
  AppCommandManager& m_command_manager;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
