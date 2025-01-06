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
 *
 * Only one action per context is allowed, i.e. context used for action registration must be unique.
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
   * @brief Sets the context to serve.
   *
   * Internally will connect proxy action with one of the real actions. If no action has been
   * registered for a given context, will disable proxy action.
   */
  void SetCurrentContext(const AppContext& current_context);

  /**
   * @brief Sets the context to serve.
   *
   * Will run through all given contexts and try to set proxy action for the first matching context.
   * If no matching action exists, will disable proxy action.
   */
  void SetContextStack(const std::vector<AppContext>& context_stack);

  /**
   * @brief Append action to the map of actions.
   */
  void AddOverrideAction(const AppContext& context, QAction* action);

  /**
   * @brief Sets text to the proxy action.
   *
   * This text defines initial appearance of the proxy action in the menu, when no underlying action
   * is defined.
   */
  AppCommand& SetText(const QString& text);

  /**
   * @brief Sets shortcut sequence.
   */
  AppCommand& SetShortcut(const QKeySequence& shortcut);

  /**
   * @brief Checks if there is a registered action for given context.
   */
  bool HasAction(const AppContext& context) const;

  /**
   * @brief Returns action registered for given context.
   */
  QAction* GetActionForContext(const AppContext& context) const;

private:
  ProxyAction* m_proxy_action{nullptr};
  std::map<AppContext, QAction*> m_context_to_action;
  QString m_default_text;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_H_
