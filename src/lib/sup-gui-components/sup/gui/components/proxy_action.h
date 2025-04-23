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

#ifndef SUP_GUI_COMPONENTS_PROXY_ACTION_H_
#define SUP_GUI_COMPONENTS_PROXY_ACTION_H_

#include <sup/gui/core/flags.h>

#include <QAction>

namespace sup::gui
{

/**
 * @brief The ProxyAction class embeds real action objects in certain contexts (menu, toolbar) and
 * adjusts its appearance.
 *
 * Use case a): copy-and-paste. A single instance of ProxyAction is added to the application's main
 * menubar and forwards user clicks to the concrete widget which is currently in focus.
 *
 * Use case b): tweak enable/disable. We want the same action to be shown both in a toolbar and in a
 * context menu. In the context menu, we want to provide context-dependent enable/disable
 * appearance, while in a toolbar, for aesthetics reasons, we want an action to be shown as always
 * enabled. In this case ProxyAction goes to a toolbar and forwards clicks to real actions. Any
 * change in enable/disable status or real actions is not visible in a toolbar.
 *
 * Inspired by https://testcase.me/2023/02/proxy-action/
 */
class ProxyAction : public QAction
{
  Q_OBJECT

public:
  enum class Options
  {
    None,
    SyncEnabledStatus  //!< follow enabled status of underlying action
  };
  using flags_t = Flags<Options>;

  explicit ProxyAction(QObject* parent = nullptr);

  /**
   * @brief Returns real action.
   */
  QAction* GetAction() const;

  /**
   * @brief Sets underlying real action.
   */
  void SetAction(QAction* action, flags_t proxy_options = Options::None);

private:
  /**
   * @brief Establishes connections with the underlying real action, or make it disconnected.
   */
  void SetConnected(bool value);

  /**
   * @brief Updates this action appearance from underlying action appearance.
   */
  void Update();

  QAction* m_action{nullptr};
  flags_t m_proxy_options;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROXY_ACTION_H_
