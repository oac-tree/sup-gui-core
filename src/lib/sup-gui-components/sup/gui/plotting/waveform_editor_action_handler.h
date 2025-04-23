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

#ifndef SUP_GUI_PLOTTING_WAVEFORM_EDITOR_ACTION_HANDLER_H_
#define SUP_GUI_PLOTTING_WAVEFORM_EDITOR_ACTION_HANDLER_H_

#include <sup/gui/plotting/waveform_editor_context.h>

#include <QObject>

namespace mvvm
{
class ISessionModel;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

struct WaveformEditorContext;

/**
 * @brief The WaveformEditorActionHandler class implements logic to add/remove points to waveform.
 */
class WaveformEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit WaveformEditorActionHandler(WaveformEditorContext context, QObject* parent = nullptr);
  ~WaveformEditorActionHandler() override;

  void OnAddColumnBeforeRequest();
  void OnAddColumnAfterRequest();
  void OnRemoveColumnRequest();

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  mvvm::ISessionModel* GetModel();
  mvvm::LineSeriesItem* GetLineSeries();
  mvvm::PointItem* GetSelectedPoint();

  mvvm::SessionItem* GetParent();

  WaveformEditorContext m_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_WAVEFORM_EDITOR_ACTION_HANDLER_H_
