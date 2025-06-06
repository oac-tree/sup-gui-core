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

#include "steady_menu.h"

namespace sup::gui
{

SteadyMenu::SteadyMenu(QWidget *parent_widget) : QMenu(parent_widget) {}

void SteadyMenu::mouseReleaseEvent(QMouseEvent *event)
{
  auto action = activeAction();

  if (action && action->isEnabled())
  {
    // do not call parent QMenu::mouseReleaseEvent to avoid menu close
    action->trigger();
  }
  else
  {
    QMenu::mouseReleaseEvent(event);
  }
}

}  // namespace sup::gui
