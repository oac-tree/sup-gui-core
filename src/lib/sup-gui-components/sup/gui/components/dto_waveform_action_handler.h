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

#ifndef SUP_GUI_COMPONENTS_DTO_WAVEFORM_ACTION_HANDLER_H_
#define SUP_GUI_COMPONENTS_DTO_WAVEFORM_ACTION_HANDLER_H_

#include <sup/gui/components/dto_waveform_editor_context.h>

#include <QObject>
#include <memory>

namespace mvvm
{
class ISessionModel;
class LineSeriesDataItem;
class LineSeriesItem;
class ChartViewportItem;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The DtoWaveformActionHandler class provides a logic to handle main actions of
 * DtoWaveformView.
 *
 * These are actions to add/remove/cut/copy/pase waveforms. Normally triggered from the left panel
 * of DtoWaveformView.
 */
class DtoWaveformActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit DtoWaveformActionHandler(DtoWaveformEditorContext context,
                                    QObject* parent_object = nullptr);

  /**
   * @brief Checks if waveform can be added to the container.
   */
  bool CanAddWaveform() const;

  /**
   * @brief Adds new waveform after current selection.
   */
  void AddWaveform();

  /**
   * @brief Checks if the waveform can be removed from the container.
   */
  bool CanRemoveWaveform() const;

  /**
   * @brief Removes currently selected waveform;
   */
  void RemoveWaveform();

signals:
  void SelectWaveformRequest(mvvm::LineSeriesItem* item);

private:
  /**
   * @brief Returns currently selected waveform.
   */
  mvvm::LineSeriesItem* GetSelectedWaveform() const;

  /**
   * @brief Returns container used to store LineSeriesItem.
   */
  mvvm::ChartViewportItem* GetWaveformContainer() const;

  /**
   * @brief Returns container used to store LineSeriesDataItem.
   */
  mvvm::SessionItem* GetDataContainer() const;

  /**
   * @brief Returns the model.
   */
  mvvm::ISessionModel* GetModel();

  /**
   * @brief Insert new waveform after current selection.
   */
  mvvm::LineSeriesItem* InsertWaveform(std::unique_ptr<mvvm::LineSeriesItem> waveform);

  /**
   * @brief Insert new data for given waveform.
   *
   * The data will be located in the data container.
   */
  void InsertDataForWaveform(mvvm::LineSeriesItem* waveform);

  DtoWaveformEditorContext m_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_WAVEFORM_ACTION_HANDLER_H_
