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

#ifndef SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
#define SUP_GUI_APP_APP_CONTEXT_MANAGER_H_

#include <QObject>
#include <map>

namespace sup::gui
{

class AppCommand;

/**
 * @brief The AppCommandManager class holds a collection of commands that can be triggered either
 * via shortcuts or menus in the global taskbar.
 */
class AppCommandManager : public QObject
{
  Q_OBJECT

public:
  explicit AppCommandManager(QObject* parent);
  ~AppCommandManager() override;

  AppCommand* RegisterCommand(const QString& context_name, const QString& command_text);

private:
  std::map<QString, AppCommand*> m_commands;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
