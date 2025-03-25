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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_

#include <mvvm/views/item_view_component_provider.h>

class QTableView;

namespace mvvm
{
class ApplicationModel;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The WaveformTableComponentProvider class provides table view with custom viewmodel, proxy
 * model and selection logic.
 *
 * It is intended for presenting a single waveform as a horizontal table with two rows corresponding
 * to (x,y) values.
 */
class WaveformTableComponentProvider : public mvvm::ItemViewComponentProvider
{
  Q_OBJECT

public:
  explicit WaveformTableComponentProvider(mvvm::ApplicationModel* model, QTableView* view);

  int GetSelectedPointIndex() const;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_
