/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_COMPONENTS_PROJECT_HANDLER_UTILS_H_
#define SUP_GUI_COMPONENTS_PROJECT_HANDLER_UTILS_H_

#include <memory>

class QAction;
class QMenu;

//! Contains utility functions to create actions and populate menus with
//! (create new, open, save, save as) requests.

namespace sup::gui
{

class ProjectHandler;

/**
 * @brief Returns an action to trigger "close current project" request.
 * Action's ownership will belong to project handler.
 */
QAction* GetNewProjectAction(ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "close current project" request.
 */
void AddNewProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Returns an action to trigger "open existing project" request.
 * Action's ownership will belong to project handler.
 */
QAction* GetOpenExistingProjectAction(ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "close current project" request.
 */
void AddOpenExistingProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Returns an action to trigger "save current project" request.
 * Action's ownership will belong to project handler.
 */
QAction* GetSaveCurrentProjectAction(ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.
 */
void AddSaveCurrentProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Returns an action to trigger "save project as" request.
 * Action's ownership will belong to project handler.
 */
QAction* GetSaveProjectAsAction(ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.
 */
void AddSaveProjectAsAction(QMenu* menu, ProjectHandler& handler);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_HANDLER_UTILS_H_
