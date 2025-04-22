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

#ifndef SUP_GUI_WIDGETS_OVERLAY_MESSAGE_FRAME_H_
#define SUP_GUI_WIDGETS_OVERLAY_MESSAGE_FRAME_H_

#include <QFont>
#include <QFrame>

#include <cstdint>

namespace sup::gui
{

/**
 * @brief The OverlayMessageFrame class represents a message box intended for overlay placement.
 *
 * It can be placed on top of any widget outside any layout context.
 */
class OverlayMessageFrame : public QFrame
{
  Q_OBJECT

public:
  explicit OverlayMessageFrame(const QString& text, QWidget* parent_widget = nullptr);

  void SetPosition(std::int32_t x, std::int32_t y);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QString m_text;
  QRect m_bounding_rect;
  QFont m_font;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_OVERLAY_MESSAGE_FRAME_H_
