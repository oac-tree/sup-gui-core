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

#include "anyvalue_editor_app_context.h"

#include <sup/gui/app/menu_bar_action_registry.h>
#include <sup/gui/app/i_action_container.h>

namespace sup::gui
{

AnyValueEditorAppContext::AnyValueEditorAppContext()
    : m_menu_bar_action_registry(std::make_unique<MenuBarActionRegistry>())
{
}

AnyValueEditorAppContext::~AnyValueEditorAppContext() = default;

IActionContainerRegistry &AnyValueEditorAppContext::GetGlobalMenuBarActionRegistry()
{
  return *m_menu_bar_action_registry;
}

}  // namespace sup::gui
