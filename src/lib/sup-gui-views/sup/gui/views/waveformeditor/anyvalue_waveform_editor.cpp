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

#include "anyvalue_waveform_editor.h"

#include "waveform_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <QVBoxLayout>

namespace sup::gui
{

AnyValueWaveformEditor::AnyValueWaveformEditor(ISettingsManager& settings_manager,
                                               QWidget* parent_widget)
    : AbstractAnyValueEditor(parent_widget), m_waveform_editor(new WaveformEditor(settings_manager))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_waveform_editor);
}

AnyValueWaveformEditor::~AnyValueWaveformEditor() = default;

void AnyValueWaveformEditor::SetInitialValue(const AnyValueItem* item)
{
  const auto waveform_points =
      (item == nullptr) ? std::vector<std::pair<double, double>>{} : GetWaveform(*item);
  m_waveform_editor->SetWaveform(waveform_points, "");
}

std::unique_ptr<AnyValueItem> AnyValueWaveformEditor::GetResult() const
{
  return CreateFromWaveform(m_waveform_editor->GetWaveform());
}

}  // namespace sup::gui
