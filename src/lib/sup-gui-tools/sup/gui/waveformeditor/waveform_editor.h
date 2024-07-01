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

#ifndef SUP_GUI_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_
#define SUP_GUI_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_

#include <QWidget>
#include <memory>

namespace mvvm
{
class ApplicationModel;
class LineSeriesItem;
class LineSeriesDataItem;
}  // namespace mvvm

namespace sup::gui
{

class WaveformEditorView;
class WaveformEditorToolBar;
class WaveformEditorActionHandler;
struct WaveformEditorContext;

class WaveformEditor : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformEditor(QWidget* parent = nullptr);
  ~WaveformEditor() override;

  void SetWaveformModel(mvvm::ApplicationModel* model);

  void SetSetpoint(mvvm::LineSeriesDataItem* data_item, const std::string& title);

private:
  void SetupConnections();
  WaveformEditorContext CreateActionContext() const;

  mvvm::ApplicationModel* m_model{nullptr};
  std::unique_ptr<WaveformEditorActionHandler> m_action_handler;

  WaveformEditorView* m_editor_view{nullptr};
  WaveformEditorToolBar* m_tool_bar{nullptr};
  mvvm::LineSeriesItem* m_line_series_item{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_
