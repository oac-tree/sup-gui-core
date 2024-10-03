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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_

#include <QWidget>

namespace sup::gui
{

class WaveformEditor;
class ItemStackWidget;

/**
 * @brief The DtoWaveformEditorPanel class is a vertical panel with embedded single-waveform editor.
 *
 * Located on the center of DtoWaveformformView.
 */
class DtoWaveformEditorPanel : public QWidget
{
  Q_OBJECT
public:
  explicit DtoWaveformEditorPanel(QWidget* parent = nullptr);
  ~DtoWaveformEditorPanel() override;


private:
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  WaveformEditor* m_waveform_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_
