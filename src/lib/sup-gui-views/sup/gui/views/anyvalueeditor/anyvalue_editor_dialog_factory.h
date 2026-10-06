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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_

#include <memory>

class QWidget;

namespace sup::gui
{

class AnyValueEditorDialog;
class AnyValueItem;
class ISettingsManager;

/**
 * @brief Creates the waveform editor dialog.
 *
 * The dialog geometry and editor settings are stored in application settings.
 */
std::unique_ptr<AnyValueEditorDialog> CreateWaveformEditorDialog(const AnyValueItem* item,
                                                                 ISettingsManager& settings_manager,
                                                                 QWidget* parent);

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
