/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

namespace sup::gui
{

void SetupDisplayName(const mvvm::SessionItem& parent, AnyValueItem& child)
{
  auto model = parent.GetModel();

  if (&parent == model->GetRootItem())
  {
    return;  // for top level items we leave display name unchanged
  }

  if (parent.GetType() == AnyValueStructItem::Type)
  {
    auto struct_item = static_cast<const AnyValueStructItem*>(&parent);
    std::string display_name = kFieldNamePrefix + std::to_string(struct_item->GetChildrenCount());
    child.SetDisplayName(display_name);
    return;
  }

  if (parent.GetType() == AnyValueArrayItem::Type)
  {
    auto struct_item = static_cast<const AnyValueArrayItem*>(&parent);
    std::string display_name = kElementNamePrefix + std::to_string(struct_item->GetChildrenCount());
    child.SetDisplayName(display_name);
    return;
  }
}

}  // namespace sup::gui
