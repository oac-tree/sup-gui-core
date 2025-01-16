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

#ifndef SUP_GUI_WIDGETS_OVERLAY_MESSAGE_H_
#define SUP_GUI_WIDGETS_OVERLAY_MESSAGE_H_

class QWidget;
class QString;

#include <memory>

namespace sup::gui
{

class OverlayWidgetPositionStrategy;

/**
 * @brief Represents a message on top of scroll area.
 */
class OverlayMessage
{
public:
  explicit OverlayMessage(const QString& text, QWidget* area);
  ~OverlayMessage();

  bool CanBeDeleted();

private:
  std::unique_ptr<QWidget> m_message_widget;
  std::unique_ptr<OverlayWidgetPositionStrategy> m_position_strategy;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_OVERLAY_MESSAGE_H_
