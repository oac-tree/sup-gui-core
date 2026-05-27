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

#ifndef SUP_GUI_WIDGETS_PLACEHOLDER_WIDGET_H_
#define SUP_GUI_WIDGETS_PLACEHOLDER_WIDGET_H_

#include <QWidget>

class QLabel;

namespace sup::gui
{

/**
 * @brief The PlaceholderWidget class shows a text in the middle of the screen.
 *
 * It is intended to substitute some other widget, when it is not ready yet. If the label contains
 * clickable links, the widget will emit a signals on the user clocks.
 */
class PlaceholderWidget : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief PlaceholderWidget
   *
   * @param header_text The text to be placed in the middle of the screen.
   * @param parent_widget The parent widget.
   */
  explicit PlaceholderWidget(const QString& header_text, QWidget* parent_widget = nullptr);

signals:
  void linkActivated(const QString& link);

private:
  QLabel* m_label{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_PLACEHOLDER_WIDGET_H_
