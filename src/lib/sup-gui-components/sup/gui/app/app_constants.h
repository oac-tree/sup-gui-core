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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_APP_APP_CONSTANTS_H_
#define SUP_GUI_APP_APP_CONSTANTS_H_

//! @file
//! Collection of constants related to the whole application.

#include <QString>

namespace sup::gui::constants
{

//! the name of common settings in the application main configuration file
const QString kAppFontSettingName = "MainWindow/font";
const QString kAppVersion = "MainWindow/version";

//! the name of the groups corresponding to main menus of the menubar
const QString kFileMenu = "&File";
const QString kEditMenu = "&Edit";
const QString kToolsMenu = "&Tools";
const QString kViewMenu = "&View";
const QString kHelpMenu = "&Help";

//! identifiers to find globally registered proxy actions located on the main application toolbar
const QString kCutCommandId = "Cut";
const QString kCopyCommandId = "Copy";
const QString kPasteCommandId = "Paste";
const QString kPasteSpecialCommandId = "Paste Special";
const QString kRemoveSelectedCommandId = "Remove selected";
const QString kDeleteCommandId = "Delete";
const QString kUndoCommandId = "Undo";
const QString kRedoCommandId = "Redo";
const QString kToggleLeftPanelCommandId = "Show/hide left panel";
const QString kToggleRightPanelCommandId = "Show/hide right panel";

//! identifiers to distinguish XML projects from different applications
const QString kAnyValueEditorApplicationType = "AnyValueEditor";
const QString kDtoEditorApplicationType = "SupDtoEditor";

}  // namespace sup::gui::constants

#endif  // SUP_GUI_APP_APP_CONSTANTS_H_
