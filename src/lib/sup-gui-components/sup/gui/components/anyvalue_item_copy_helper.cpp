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

#include "anyvalue_item_copy_helper.h"

#include "copy_and_paste_helper.h"
#include "mime_conversion_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/utils/container_utils.h>

#include <QMimeData>

namespace sup::gui
{

std::unique_ptr<QMimeData> CreateAnyValueItemSelectionCopyMimeData(
    const std::vector<AnyValueItem *> &selection)
{
  // accept all items that are AnyValueItem and in the selection list, accept all properties
  auto filter_func = [&selection](const mvvm::SessionItem &item)
  {
    if (item.GetTagIndex().GetTag() == constants::kAnyValueChildrenTag)
    {
      return mvvm::utils::Contains(selection, &item);
    }
    return true;  // all properties
  };

  // FIXME Find the way to fix this CastItems/MakeConst mess
  auto top_level_selection =
      sup::gui::GetTopLevelSelection(mvvm::utils::CastItems<mvvm::SessionItem>(selection));
  return sup::gui::CreateCopyMimeData(mvvm::utils::MakeConst(top_level_selection),
                                      kCopyAnyValueMimeType, filter_func);
}

std::vector<std::unique_ptr<mvvm::SessionItem> > CreateAnyValueItems(const QMimeData *mime_data)
{
  return sup::gui::CreateSessionItems(mime_data, kCopyAnyValueMimeType);
}

}  // namespace sup::gui
