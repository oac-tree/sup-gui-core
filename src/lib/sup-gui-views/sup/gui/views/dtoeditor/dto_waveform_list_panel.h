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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_

#include <QWidget>

class QListView;

namespace mvvm
{
class ISessionModel;
}

namespace sup::gui
{

class ItemStackWidget;

/**
 * @brief The DtoWaveformListPanel class represents a vertical panel with the list of available
 * waveforms.
 *
 * Located on the left of DtoWaveformformView.
 */
class DtoWaveformListPanel : public QWidget
{
  Q_OBJECT
public:
  explicit DtoWaveformListPanel(mvvm::ISessionModel* model, QWidget* parent = nullptr);
  ~DtoWaveformListPanel() override;

private:
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  QListView* m_list_view{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_
