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

#ifndef SUP_GUI_APP_APP_CONSTANTS_H_
#define SUP_GUI_APP_APP_CONSTANTS_H_

//! @file
//! Collection of constants related to the whole application.

namespace sup::gui::constants
{

//! the name of common settings in the application main configuration file
constexpr auto kAppFontSettingName = "MainWindow/font";
constexpr auto kAppVersion = "MainWindow/version";

//! the name of the groups corresponding to main menus of the menubar
constexpr auto kFileMenu = "&File";
constexpr auto kEditMenu = "&Edit";
constexpr auto kToolsMenu = "&Tools";
constexpr auto kViewMenu = "&View";
constexpr auto kHelpMenu = "&Help";

//! identifiers to find globally registered proxy actions located on the main application toolbar
constexpr auto kCutCommandId = "Cut";
constexpr auto kCopyCommandId = "Copy";
constexpr auto kPasteCommandId = "Paste";
constexpr auto kPasteSpecialCommandId = "Paste Special";
constexpr auto kRemoveSelectedCommandId = "Remove selected";
constexpr auto kDeleteCommandId = "Delete";
constexpr auto kUndoCommandId = "Undo";
constexpr auto kRedoCommandId = "Redo";
constexpr auto kToggleLeftPanelCommandId = "Show/hide left panel";
constexpr auto kToggleRightPanelCommandId = "Show/hide right panel";

//! identifiers to distinguish XML projects from different applications
constexpr auto kAnyValueEditorApplicationType = "AnyValueEditor";
constexpr auto kDtoEditorApplicationType = "SupDtoEditor";

}  // namespace sup::gui::constants

#endif  // SUP_GUI_APP_APP_CONSTANTS_H_
