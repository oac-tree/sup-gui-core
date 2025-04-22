/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "visibility_agent_base.h"

#include <QEvent>

namespace sup::gui
{

VisibilityAgentBase::VisibilityAgentBase(QObject *parent_object, callback_t subscribe,
                                         callback_t unsubscribe)
    : QObject(parent_object)
    , m_subscribe_callback(std::move(subscribe))
    , m_unsubscribe_callback(std::move(unsubscribe))
{
  parent_object->installEventFilter(this);
}

bool VisibilityAgentBase::eventFilter(QObject *obj, QEvent *event)
{
  (void)obj;

  if (event->type() == QEvent::Show)
  {
    m_subscribe_callback();
  }
  else if (event->type() == QEvent::Hide)
  {
    m_unsubscribe_callback();
  }

  return false;  // let the parent handle it normally
}

}  // namespace sup::gui
