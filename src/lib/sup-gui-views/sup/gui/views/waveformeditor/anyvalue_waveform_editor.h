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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_ANYVALUE_WAVEFORM_EDITOR_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_ANYVALUE_WAVEFORM_EDITOR_H_

#include <sup/gui/views/anyvalueeditor/abstract_anyvalue_editor.h>

namespace sup::gui
{

class WaveformEditor;

/**
 * @brief The AnyValueWaveformEditor class is a wrapper widget around WaveformEditor to edit
 * AnyValueItems intended for waveform storage.
 *
 * The initial waveform and result of waveform editing are encoded in AnyValueItem. Underlying
 * editor operates via mvvm::LineSeriesItem machinery.
 */
class AnyValueWaveformEditor : public AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueWaveformEditor(QWidget* parent_widget = nullptr);
  ~AnyValueWaveformEditor() override;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() override;

private:
  WaveformEditor* m_waveform_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_ANYVALUE_WAVEFORM_EDITOR_H_
