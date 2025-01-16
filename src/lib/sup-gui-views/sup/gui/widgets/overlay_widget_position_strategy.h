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

#ifndef SUP_GUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_
#define SUP_GUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_

#include <QFrame>

class QWidget;
class QPoint;

namespace sup::gui
{

/**
 * @brief The OverlayWidgetPositionStrategy class controls the appearance of an overlay widget on
 * top of another widget.
 *
 * The current implementation places controlled widget at the lower right corner of the main widget
 * (hardcoded behavior).
 */
class OverlayWidgetPositionStrategy : public QObject
{
  Q_OBJECT

public:
  explicit OverlayWidgetPositionStrategy(QWidget* overlay_widget, QWidget* area,
                                         QObject* parent_object = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  void UpdateOverlayWidgetGeometry();

  /**
   * @brief Calculates the position of the message widget to be approximately at the lower right
   * corner of the area.
   *
   * Takes into account scroll bars if exist.
   */
  QPoint CalculateWidgetPosition() const;

  QWidget* m_overlay_widget{nullptr};
  QWidget* m_area{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_OVERLAY_WIDGET_POSITION_STRATEGY_H_
