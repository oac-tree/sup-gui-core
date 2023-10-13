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

#ifndef SUP_GUI_ANYVALUEEDITOR_CUSTOM_METATYPES_H
#define SUP_GUI_ANYVALUEEDITOR_CUSTOM_METATYPES_H

//! Registration of custom types in Qt meta type system to provide support in signal/slot and
//! QVariant.

#include <QMetaType>

namespace sup::gui
{

void RegisterCustomMetaTypes();

}  // namespace sequencergui

#endif  // SUP_GUI_ANYVALUEEDITOR_CUSTOM_METATYPES_H
