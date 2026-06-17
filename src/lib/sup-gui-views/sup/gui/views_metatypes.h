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

#ifndef SUP_GUI_VIEWS_METATYPES_H_
#define SUP_GUI_VIEWS_METATYPES_H_

//! @file
//! Registration of custom types in Qt meta type system to provide support in signal/slot.

namespace sup::gui::views
{

void RegisterCustomMetaTypes();

}  // namespace sup::gui::views

#endif  // SUP_GUI_VIEWS_METATYPES_H_
