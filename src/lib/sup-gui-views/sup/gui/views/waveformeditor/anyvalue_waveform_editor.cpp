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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_waveform_editor.h"

#include "waveform_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <QVBoxLayout>

namespace sup::gui
{

AnyValueWaveformEditor::AnyValueWaveformEditor(QWidget *parent_widget)
    : AbstractAnyValueEditor(parent_widget), m_waveform_editor(new WaveformEditor(nullptr))

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_waveform_editor);
}

AnyValueWaveformEditor::~AnyValueWaveformEditor() = default;

void AnyValueWaveformEditor::SetInitialValue(const AnyValueItem *item)
{
  // FIXME introduce check that AnyValueItem is suitable for waveform
  m_waveform_editor->SetWaveform(GetWaveform(dynamic_cast<const AnyValueArrayItem *>(item)), "");
}

std::unique_ptr<AnyValueItem> AnyValueWaveformEditor::GetResult()
{
  return CreateFromWaveform(m_waveform_editor->GetWaveform());
}

}  // namespace sup::gui
