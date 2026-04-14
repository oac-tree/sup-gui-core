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

#ifndef SUP_GUI_WIDGETS_VISIBILITY_AGENT_BASE_H_
#define SUP_GUI_WIDGETS_VISIBILITY_AGENT_BASE_H_

#include <QObject>
#include <functional>

namespace sup::gui
{

/**
 * @brief The VisibilityAgentBase class calls two different callbacks depending on whether the
 * target widget is visible or not.
 *
 * It is used to unsubscribe from model notifications when widget is hidden.
 */

class VisibilityAgentBase : public QObject
{
  Q_OBJECT

public:
  using callback_t = std::function<void()>;

  VisibilityAgentBase(callback_t subscribe, callback_t unsubscribe);
  VisibilityAgentBase(QObject* parent_object, callback_t subscribe, callback_t unsubscribe);

  /**
   * @brief SetTarget sets the widget to monitor for visibility changes.
   *
   * The callbacks will be called when the widget is shown or hidden.
   */
  void SetTarget(QObject* widget);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  callback_t m_subscribe_callback;
  callback_t m_unsubscribe_callback;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_VISIBILITY_AGENT_BASE_H_
