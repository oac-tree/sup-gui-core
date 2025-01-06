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

#include "sup_dto_model.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

namespace sup::gui
{

SupDtoModel::SupDtoModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("SupDtoModel", std::move(pool))

{
  PopulateModel();
}

void SupDtoModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

std::vector<mvvm::ContainerItem*> SupDtoModel::GetContainers() const
{
  return mvvm::utils::GetTopItems<mvvm::ContainerItem>(this);
}

void SupDtoModel::PopulateModel()
{
  (void)InsertItem<mvvm::ContainerItem>();
  if (auto command_stack = GetCommandStack(); command_stack)
  {
    command_stack->Clear();  // to disallow "undo" of our main container
  }
}

}  // namespace sup::gui
