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

#ifndef SUP_GUI_APP_APP_CONSTANTS_H_
#define SUP_GUI_APP_APP_CONSTANTS_H_

//! @file
//! Collection of constants related to a global action registration.

#include <QString>

namespace sup::gui::constants
{

// Identifiers to find globally registered proxy actions located on the main application toolbar.
const QString kCutCommandId = "Cut";
const QString kCopyCommandId = "Copy";
const QString kPasteCommandId = "Paste";
const QString kPasteSpecialCommandId = "Paste Special";

}  // namespace sup::gui::constants

#endif  // SUP_GUI_APP_APP_ACTION_HELPER_H_
