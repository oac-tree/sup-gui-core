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

#ifndef SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_
#define SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_

#include <string>
#include <QStringList>

class QWidget;

namespace mvvm
{
enum class SaveChangesAnswer;
}

namespace sup::gui
{

class RecentProjectSettings;

//! Provide save/discard/cancel and similar dialogs on user request.

class ProjectUserInteractor
{
public:
  ProjectUserInteractor(sup::gui::RecentProjectSettings* settings, QWidget* parent);

  std::string OnSelectDirRequest();

  std::string OnCreateDirRequest();

  mvvm::SaveChangesAnswer OnSaveChangesRequest();

private:
  std::string SummonSelectDialog() const;

  sup::gui::RecentProjectSettings* m_settings{nullptr};
  QWidget* m_parent{nullptr};

  QString m_current_workdir;
  QStringList m_recent_projects;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_
