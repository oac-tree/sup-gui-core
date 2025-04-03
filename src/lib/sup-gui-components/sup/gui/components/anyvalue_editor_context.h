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

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_CONTEXT_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_CONTEXT_H_

#include <sup/gui/core/message_event.h>

#include <functional>
#include <memory>

class QMimeData;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace sup::gui
{

/**
 * @brief The AnyValueEditorContext class holds collection of callbacks to retrieve current state of
 * AnyValueEditor.
 */
struct AnyValueEditorContext
{
  //!< callback to retrieve currently selected AnyValueItem
  std::function<sup::gui::AnyValueItem*()> selected_items;

  //!< A callback to notify the user that the item should be selected in a widget. This can
  //!< happen when a new item was just inserted, or some other item should be selected instead of
  //!< just removed.
  std::function<void(mvvm::SessionItem*)> notify_request;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message;

  //! callback to get mime data from the clipboard
  std::function<const QMimeData*()> get_mime_data;

  //! callback to set mime data to the clipboard
  std::function<void(std::unique_ptr<QMimeData>)> set_mime_data;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_CONTEXT_H_
