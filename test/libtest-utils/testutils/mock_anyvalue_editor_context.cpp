/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "mock_anyvalue_editor_context.h"

#include <sup/gui/components/anyvalue_editor_action_handler.h>
#include <sup/gui/model/anyvalue_item.h>

#include <QMimeData>

namespace sup::gui::test
{

AnyValueEditorContext MockAnyValueEditorContext::CreateContext(
    const std::vector<sup::gui::AnyValueItem*>& current_selection)
{
  AnyValueEditorContext result;

  m_current_selection = current_selection;

  result.selected_items = [this]() { return m_current_selection; };

  result.notify_request = [this](auto item)
  {
    m_notify_request.push_back(item);
    NotifyRequest(item);
  };
  result.send_message = [this](const auto& message) { OnMessage(message); };

  result.get_mime_data = [this]()
  {
    OnGetMimeData();
    return m_clipboard_content.get();
  };

  result.set_mime_data = [this](auto mime_data)
  {
    // mimicking clipboard, saving copy result here
    m_clipboard_content = std::move(mime_data);
    OnSetMimeData();
  };

  return result;
}

std::unique_ptr<AnyValueEditorActionHandler> MockAnyValueEditorContext::CreateActionHandler(
    mvvm::SessionItem* container, const std::vector<AnyValueItem*>& current_selection)
{
  return std::make_unique<AnyValueEditorActionHandler>(CreateContext(current_selection), container,
                                                       nullptr);
}


QMimeData* MockAnyValueEditorContext::GetClipboardContent() const
{
  return m_clipboard_content.get();
}

void MockAnyValueEditorContext::SetClipboardContent(std::unique_ptr<QMimeData> clipboard)
{
  m_clipboard_content = std::move(clipboard);
}

std::vector<mvvm::SessionItem*> MockAnyValueEditorContext::GetNotifyRequests()
{
  return m_notify_request;
}

void MockAnyValueEditorContext::SetAsCurrentSelection(
    const std::vector<sup::gui::AnyValueItem*>& selection)
{
  m_current_selection = selection;
}

}  // namespace sup::gui::test
