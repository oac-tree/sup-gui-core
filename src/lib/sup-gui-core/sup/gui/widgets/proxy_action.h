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

#ifndef SUP_GUI_WIDGETS_PROXY_ACTION_H_
#define SUP_GUI_WIDGETS_PROXY_ACTION_H_

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
 * enabled. In this case ProxyAction will go to a toolbar.
 */
class ProxyAction : public QAction
{
  Q_OBJECT

public:
  explicit ProxyAction(QAction* action, QObject* parent = nullptr);

private:
  QAction* m_action{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_PROXY_ACTION_H_
