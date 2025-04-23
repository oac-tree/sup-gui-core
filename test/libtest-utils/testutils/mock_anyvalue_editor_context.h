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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_ANYVALUE_EDITOR_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_ANYVALUE_EDITOR_CONTEXT_H_

#include <sup/gui/components/anyvalue_editor_context.h>

#include <gmock/gmock.h>

#include <memory>

namespace sup::gui
{
class AnyValueEditorActionHandler;
}

namespace sup::gui::test
{

/**
 * @brief The MockAnyValueEditorContext creates and listens callbacks used by
 * AnyValueEditorActionHandler.
 */
class MockAnyValueEditorContext
{
public:
  MOCK_METHOD(void, NotifyRequest, (mvvm::SessionItem *), ());
  MOCK_METHOD(void, OnMessage, (const sup::gui::MessageEvent &), ());
  MOCK_METHOD(const QMimeData *, OnGetMimeData, (), ());
  MOCK_METHOD(void, OnSetMimeData, (), ());

  /**
   * @brief Creates context for AnyValueEditorActionHandler.
   *
   * @param current_selection Items which will be reported as selected by the user.
   */
  AnyValueEditorContext CreateContext(const std::vector<AnyValueItem *> &current_selection);

  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      mvvm::SessionItem* container,
      const std::vector<sup::gui::AnyValueItem *> &current_selection);

  QMimeData *GetClipboardContent() const;

  void SetClipboardContent(std::unique_ptr<QMimeData> clipboard);

  std::vector<mvvm::SessionItem *> GetNotifyRequests();

  void SetAsCurrentSelection(const std::vector<AnyValueItem *> &selection);

  std::unique_ptr<QMimeData> m_clipboard_content;
  std::vector<sup::gui::AnyValueItem *> m_current_selection;
  std::vector<mvvm::SessionItem *> m_notify_request;
};

}  // namespace sup::gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_ANYVALUE_EDITOR_CONTEXT_H_
