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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_

//! Helper functions for AnyValueEditor.

#include <string>

namespace mvvm
{
class ApplicationModel;
class SessionItem;
}

namespace sup::gui
{
class AnyValueItem;

/**
 * @brief Sets appropriate display name for a child.
 * @param parent A parent where a child will be inserted.
 * @param child Future child of a parent
 */
void SetupDisplayName(const mvvm::SessionItem& parent, AnyValueItem& child);

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_
