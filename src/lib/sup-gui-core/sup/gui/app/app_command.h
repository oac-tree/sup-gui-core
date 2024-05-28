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

#ifndef SUP_GUI_APP_APP_COMMAND_H_
#define SUP_GUI_APP_APP_COMMAND_H_

#include <sup/gui/app/app_context.h>

#include <QObject>
#include <map>

class QKeySequence;
class QAction;

namespace sup::gui
{

class ProxyAction;

/**
 * @brief The AppCommand class is an envelope around a proxy action and a list of actions to
 * forward.
 *
 * Proxy action is executed either via shortcut or is triggered from the global menubar. Depending
 * on the context, a proxy action will trigger one of the actions in the list.
 */
class AppCommand : public QObject
{
  Q_OBJECT

public:
  explicit AppCommand(const QString& text, QObject* parent = nullptr);
  AppCommand(const QString& text, const QKeySequence& key, QObject* parent = nullptr);
  ~AppCommand() override;

  /**
   * @brief Returns underying proxy action.
   *
   * A proxy action is intended for main taskbars.
   */
  ProxyAction* GetProxyAction();

  /**
   * @brief Set the context to serve.
   *
   * Internally will connect proxy action with one of the real actions. If more than one action fits
   * given context, will ignore the rest.
   */
  void SetCurrentContext(const AppContext& current_context);

  void SetCurrentContextWidget(const QWidget* widget);

  /**
   * @brief Append action to the map of actions.
   */
  void AddOverrideAction(const AppContext& context, QAction* action);

  /**
   * @brief Sets shortcut sequence.
   */
  void SetKeySequence(const QKeySequence& shortcut);

private:
  ProxyAction* m_proxy_action{nullptr};
  std::map<QAction*, AppContext> m_action_to_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_H_
