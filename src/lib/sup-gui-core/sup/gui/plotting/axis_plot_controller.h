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

#ifndef SUP_GUI_PLOTTING_AXIS_PLOT_CONTROLLER_H_
#define SUP_GUI_PLOTTING_AXIS_PLOT_CONTROLLER_H_

#include <mvvm/signals/item_listener.h>

#include <QObject>
#include <memory>

namespace QtCharts
{
class QAbstractAxis;
}

namespace mvvm
{
class ViewportAxisItem;
}

namespace sup::gui
{

/**
 * @brief The AxisPlotController class establishes communications between QtCharts::QAbstractAxis
 * and mvvm::ViewportAxisItem.
 *
 * @details It provides mutual updates of axis parameters (min, max) between two axes
 * representations.
 */

class AxisPlotController : public mvvm::ItemListener<mvvm::ViewportAxisItem>
{
public:
  explicit AxisPlotController(QtCharts::QAbstractAxis* axis);

  QtCharts::QAbstractAxis* GetQtAxis();

  void Subscribe() override;
  void Unsubscribe() override;

private:
  /**
   * @brief Connects Qt signals.
   */
  void SetQtConnected();

  /**
   * @brief Disconnects Qt signals.
   */
  void SetQtDisonnected();

  /**
   * @brief Set min, max values of Qt axis.
   */
  void SetAxisRangeFromItem();

  /**
   * @brief Update Qt axis when some of item's properties has changed.
   */
  void OnPropertyChangedEvent(const mvvm::PropertyChangedEvent& event);

  QtCharts::QAbstractAxis* m_axis{nullptr};

  //! Special connection with Qt axis is necessary since AxisPlotController is not a QObject.
  std::unique_ptr<QMetaObject::Connection> m_axis_connection;
  bool m_block_update_from_item{false};
};

}  // namespace pspsdemo

#endif  // SUP_GUI_PLOTTING_AXIS_PLOT_CONTROLLER_H_
