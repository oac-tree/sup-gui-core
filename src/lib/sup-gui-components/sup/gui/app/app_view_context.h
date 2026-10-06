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

#ifndef SUP_GUI_APP_APP_VIEW_CONTEXT_H_
#define SUP_GUI_APP_APP_VIEW_CONTEXT_H_

namespace sup::gui
{

class IAppCommandService;
class ISettingsManager;

/**
 * @brief The AppViewContext struct holds application services shared by views.
 *
 * The services must outlive every view using the context.
 */
struct AppViewContext
{
  IAppCommandService& command_service;  //!< global commands and their shortcuts
  ISettingsManager& settings_manager;   //!< widget settings
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_VIEW_CONTEXT_H_
