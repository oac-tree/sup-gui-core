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

#include "dto_composer_action_handler.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/standarditems/container_item.h>

namespace sup::gui
{

DtoComposerActionHandler::DtoComposerActionHandler(mvvm::SessionModelInterface *model,
                                                   QObject *parent)
    : QObject(parent), m_model(model)
{
  if (!m_model)
  {
    throw RuntimeException("DtoComposerActionHandler: model is not initialised");
  }
}

void DtoComposerActionHandler::OnRemoveContainer(int container_index)
{
  m_model->TakeItem(m_model->GetRootItem(), mvvm::TagIndex::Default(container_index));
}

void DtoComposerActionHandler::OnAddNewContainer()
{
  m_model->InsertItem<mvvm::ContainerItem>();
}

}  // namespace sup::gui
