/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#ifndef SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_APP_CONTEXT_H_
#define SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_APP_CONTEXT_H_

#include <memory>

namespace sup::gui
{

class IAppCommandService;

/**
 * @brief The AnyValueEditorAppContext contains all common resources for AnyValueEditor application.
 */
class AnyValueEditorAppContext
{
public:
  AnyValueEditorAppContext();
  ~AnyValueEditorAppContext();

  AnyValueEditorAppContext(const AnyValueEditorAppContext&) = delete;
  AnyValueEditorAppContext& operator=(const AnyValueEditorAppContext&) = delete;
  AnyValueEditorAppContext(AnyValueEditorAppContext&&) = delete;
  AnyValueEditorAppContext& operator=(AnyValueEditorAppContext&&) = delete;

  IAppCommandService& GetCommandService();

private:
  std::unique_ptr<IAppCommandService> m_command_service;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_APP_CONTEXT_H_
