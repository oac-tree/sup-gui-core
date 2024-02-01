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

#include "dto_composer_actions.h"

#include <QAction>

namespace sup::gui
{

DtoComposerActions::DtoComposerActions(QObject *parent) : QObject(parent)
{
  m_remove_current_tab_action = new QAction("Remove current tab", this);
  connect(m_remove_current_tab_action, &QAction::triggered, this,
          &DtoComposerActions::RemoveCurrentTabRequest);

  m_add_new_tab_action = new QAction("Add new tab", this);
  connect(m_add_new_tab_action, &QAction::triggered, this, &DtoComposerActions::AddNewTabRequest);
}

QList<QAction *> DtoComposerActions::GetActions() const
{
  return QList<QAction *>({m_add_new_tab_action, m_remove_current_tab_action});
}

}  // namespace sup::gui
