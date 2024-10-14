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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_

#include <memory>

class QWidget;

namespace sup::gui
{

class AnyValueEditorDialog;
class AnyValueItem;

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateWaveformEditorDialog(
    const sup::gui::AnyValueItem *item, QWidget *parent);

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
