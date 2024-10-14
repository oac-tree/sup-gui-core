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

#include "anyvalue_editor_dialog_factory.h"

#include "anyvalue_editor_dialog.h"

#include <sup/gui/views/waveformeditor/anyvalue_waveform_editor.h>

namespace sup::gui
{

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateWaveformEditorDialog(
    const sup::gui::AnyValueItem *item, QWidget *parent)
{
  auto editor = std::make_unique<sup::gui::AnyValueWaveformEditor>();
  editor->SetInitialValue(item);
  auto result = std::make_unique<sup::gui::AnyValueEditorDialog>(std::move(editor), parent);
  result->setWindowTitle("Import Waveform");
  return result;
}

}  // namespace sup::gui
