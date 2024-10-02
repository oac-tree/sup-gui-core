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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_

#include <QWidget>

namespace sup::gui
{

/**
 * @brief The DtoWaveformView class represents a main view to edit multiple waveforms.
 */
class DtoWaveformView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoWaveformView(QWidget* parent = nullptr);
  ~DtoWaveformView() override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_
