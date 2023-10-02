/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#ifndef SUP_GUI_MODEL_ANYVALUE_CLIPBOARD_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_CLIPBOARD_UTILS_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <QClipboard>
#include <QGuiApplication>

namespace sup::gui
{

//! Returns a AnyValue from the clipboard
//! If the clipboard contains a list of AnyValue the first is returned
//! If the clipboard does not contain a valid AnyValue an empty AnyValue is returned
sup::dto::AnyValue ClipboardGetAnyvalue(QClipboard::Mode mode = QClipboard::Mode::Clipboard,
                                        QClipboard* clipboard = QGuiApplication::clipboard());

//! Returns a list of AnyValue from the clipboard
//! If the clipboard does not contain a valid AnyValue list an empty list is returned
std::vector<sup::dto::AnyValue> ClipboardGetAnyvalueList(
    QClipboard::Mode mode = QClipboard::Mode::Clipboard,
    QClipboard* clipboard = QGuiApplication::clipboard());

//! Stores a AnyValue in the clipboard
void ClipboardSetAnyvalue(const sup::dto::AnyValue& anyvalue,
                          QClipboard::Mode mode = QClipboard::Mode::Clipboard,
                          QClipboard* clipboard = QGuiApplication::clipboard());

//! Stores a list of AnyValue in the clipboard
void ClipboardSetAnyvalueList(const std::vector<sup::dto::AnyValue>& anyvalues,
                              QClipboard::Mode mode = QClipboard::Mode::Clipboard,
                              QClipboard* clipboard = QGuiApplication::clipboard());

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_CLIPBOARD_UTILS_H_
