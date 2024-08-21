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

#include "anyvalue_viewmodel.h"

#include "custom_children_strategies.h"
#include "custom_row_strategies.h"

#include <mvvm/viewmodel/viewmodel_controller_factory.h>

namespace sup::gui
{

AnyValueViewModel::AnyValueViewModel(mvvm::ISessionModel *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<AnyValueChildrenStrategy, AnyValueRowStrategy>(model, this));
}

}  // namespace sup::gui
