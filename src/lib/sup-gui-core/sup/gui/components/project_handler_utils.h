/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

class QAction;
class QMenu;

//! Contains utility functions to populate menus with (create new, open, save, save as) requests.

namespace sup::gui
{

class ProjectHandler;

/**
 * @brief Add to the menu an action to trigger "close current project" request.
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddNewProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "close current project" request.
 */
void AddOpenExistingProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddSaveCurrentProjectAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Add to the menu an action to trigger "save current project" request.
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddSaveProjectAsAction(QMenu* menu, ProjectHandler& handler);

/**
 * @brief Add to the menu entries to open recent projects.
 * @param menu The menu to populate with entries.
 * @param handler Project handler to use for actual job.
 */
void AddRecentProjectActions(QMenu* menu, ProjectHandler& handler);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_HANDLER_UTILS_H_
