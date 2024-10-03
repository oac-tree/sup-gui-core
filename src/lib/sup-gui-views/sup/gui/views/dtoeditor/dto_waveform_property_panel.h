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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_

#include <QWidget>

namespace sup::gui
{

/**
 * @brief The DtoWaveformPropertyPanel class is a vertical panel with waveform tree-like property
 * editors.
 *
 * Located on the right of DtoWaveformformView.
 */
class DtoWaveformPropertyPanel : public QWidget
{
  Q_OBJECT
public:
  explicit DtoWaveformPropertyPanel(QWidget* parent = nullptr);
  ~DtoWaveformPropertyPanel() override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_
