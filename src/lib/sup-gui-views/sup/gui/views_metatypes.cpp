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

#include "views_metatypes.h"

#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>

#include <mutex>

namespace sup::gui::views
{

void RegisterCustomMetaTypes()
{
  static std::once_flag register_once_flag;
  auto register_func = []()
  { (void)qRegisterMetaType<sup::gui::SplittableComboPanel*>("sup::gui::SplittableComboPanel*"); };

  std::call_once(register_once_flag, register_func);
}

}  // namespace sup::gui::views
