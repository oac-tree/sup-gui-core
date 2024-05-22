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

#include "abstract_project_user_interactor.h"

#include "message_event.h"
#include "message_helper.h"

#include <mvvm/project/project_context.h>
#include <mvvm/utils/file_utils.h>

#include <QMessageBox>
#include <map>

namespace
{
/**
 * @brief Creates a map to convert standard Qt answers to what ProjectManager expects.
 */
std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> CreateMapOfAnswers()
{
  std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> result = {
      {QMessageBox::Save, mvvm::SaveChangesAnswer::kSave},
      {QMessageBox::Discard, mvvm::SaveChangesAnswer::kDiscard},
      {QMessageBox::Cancel, mvvm::SaveChangesAnswer::kCancel}};
  return result;
}
}  // namespace

namespace sup::gui
{

AbstractProjectUserInteractor::AbstractProjectUserInteractor(const QString &application_type,
                                                             QWidget *parent)
    : m_application_type(application_type), m_parent(parent)
{
}

std::string AbstractProjectUserInteractor::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

void AbstractProjectUserInteractor::SetCurrentWorkdir(const std::string &path)
{
  m_current_workdir = path;
}

std::string AbstractProjectUserInteractor::GetNewProjectPath() const
{
  auto result = GetNewProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

std::string AbstractProjectUserInteractor::GetExistingProjectPath() const
{
  auto result = GetExistingProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

mvvm::SaveChangesAnswer AbstractProjectUserInteractor::OnSaveCurrentChangesRequest() const
{
  static auto translate_map = CreateMapOfAnswers();

  QMessageBox msgBox;
  msgBox.setText("The project has been modified.");
  msgBox.setInformativeText("Do you want to save your changes?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  auto ret = static_cast<QMessageBox::StandardButton>(msgBox.exec());
  return translate_map[ret];
}

void AbstractProjectUserInteractor::SendMessage(const std::string &message) const
{
  MessageEvent event;
  event.title = "Can't open the file";
  event.text = "Exception was caught while trying to open the file";
  event.informative = message;
  SendWarningMessage(event);
}

void AbstractProjectUserInteractor::SetUseNativeDialog(bool value)
{
  m_use_native_dialogs = value;
}

bool AbstractProjectUserInteractor::GetUseNativeDialogFlag() const
{
  return m_use_native_dialogs;
}

mvvm::UserInteractionContext AbstractProjectUserInteractor::CreateContext() const
{
  mvvm::UserInteractionContext result;
  result.existing_path_callback = [this]() { return GetExistingProjectPath(); };
  result.new_path_callback = [this]() { return GetNewProjectPath(); };
  result.answer_callback = [this]() { return OnSaveCurrentChangesRequest(); };
  result.message_callback = [this](const std::string &text) { SendMessage(text); };

  return result;
}

void AbstractProjectUserInteractor::UpdateCurrentWorkdir(const std::string &path) const
{
  if (!path.empty())
  {
    // updating mutable variable from const method
    m_current_workdir = mvvm::utils::GetParentPath(path);
  }
}

QString AbstractProjectUserInteractor::GetApplicationType() const
{
  return m_application_type;
}

QWidget *AbstractProjectUserInteractor::GetParent() const
{
  return m_parent;
}

}  // namespace sup::gui
