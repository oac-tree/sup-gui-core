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

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

namespace sup::gui
{

DtoComposerActionHandler::DtoComposerActionHandler(QObject *parent)
    : DtoComposerActionHandler(nullptr, parent)
{
}

DtoComposerActionHandler::DtoComposerActionHandler(mvvm::ISessionModel *model, QObject *parent)
    : QObject(parent), m_model(model)
{
}

void DtoComposerActionHandler::SetModel(mvvm::ISessionModel *model)
{
  m_model = model;
}

void DtoComposerActionHandler::OnRemoveContainer(int container_index)
{
  ValidateModel();
  m_model->TakeItem(m_model->GetRootItem(), mvvm::TagIndex::Default(container_index));
}

void DtoComposerActionHandler::OnAddNewContainer()
{
  ValidateModel();
  m_model->InsertItem<mvvm::ContainerItem>();
}

void DtoComposerActionHandler::OnDuplicateContainer(int container_index)
{
  ValidateModel();
  auto container_to_copy =
      m_model->GetRootItem()->GetItem(mvvm::TagIndex::Default(container_index));

  // copy container right after the given index
  mvvm::utils::CopyItem(container_to_copy, m_model, m_model->GetRootItem(),
                        container_to_copy->GetTagIndex().Next());
}

void DtoComposerActionHandler::ValidateModel()
{
  if (!m_model)
  {
    throw RuntimeException("DtoComposerActionHandler: model is not initialised");
  }
}

}  // namespace sup::gui
