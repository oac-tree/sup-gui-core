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

#ifndef SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_
#define SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_

#include <QStringList>
#include <memory>
#include <string>

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
  explicit ProjectUserInteractor(QWidget* parent);
  ~ProjectUserInteractor();

  std::string OnSelectDirRequest();

  std::string OnCreateDirRequest();

  mvvm::SaveChangesAnswer OnSaveChangesRequest();

  QStringList GetRecentProjectList();

  void AddToRecentProjectList(const QString& project_dir_name);

  void ClearRecentProjectsList();

  /**
   * @brief Sets the flag responsible for using system native file/directory selection dialogs.
   *
   * @details When true, will use system native dialogs, if the system has one (default). When
   * false, will use Qt's own dialog.
   */
  void SetUseNativeDialog(bool value);

private:
  std::string SummonSelectDialog(const QString& title) const;

  std::unique_ptr<sup::gui::RecentProjectSettings> m_settings;
  QWidget* m_parent{nullptr};
  bool m_use_native_dialogs{true};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_USER_INTERACTOR_H_
