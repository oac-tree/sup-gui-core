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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_TOOLBAR_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_TOOLBAR_H_

#include <QToolBar>

class QToolButton;

namespace sup::gui
{

class WaveformEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit WaveformEditorToolBar(QWidget* parent = nullptr);

signals:
  void AddColumnBeforeRequest();
  void AddColumnAfterRequest();
  void RemoveColumnRequest();
  void ZoomInRequest();
  void ZoomOutRequest();
  void SetViewportToContentRequest();

private:
  QToolButton* m_add_column_before{nullptr};
  QToolButton* m_add_column_after{nullptr};
  QToolButton* m_remove_column{nullptr};
  QToolButton* m_zoom_in{nullptr};
  QToolButton* m_zoom_out{nullptr};
  QToolButton* m_center_canvas{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_TOOLBAR_H_
