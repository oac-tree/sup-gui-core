/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_MAINWINDOW_SESSION_ITEM_WIDGET_H_
#define SUP_GUI_MAINWINDOW_SESSION_ITEM_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class SessionItem;
class ISessionModel;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The SessionItemWidget class is a base for all widgets which can show one item at a time.
 */
class SessionItemWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SessionItemWidget(QWidget* parent_widget = nullptr);

  /**
   * @brief Sets the model.
   *
   * The default implementation does nothing. Some widgets might want to implement
   * model listeners and react to model changes.
   */
  virtual void SetModel(mvvm::ISessionModel* model);

  /**
   * @brief Sets the item to be served by this widget.
   */
  virtual void SetItem(mvvm::SessionItem* item) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SESSION_ITEM_WIDGET_H_
