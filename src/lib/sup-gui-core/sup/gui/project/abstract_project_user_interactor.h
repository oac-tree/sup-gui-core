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

#ifndef SUP_GUI_COMPONENTS_ABSTRACT_PROJECT_USER_INTERACTOR_H_
#define SUP_GUI_COMPONENTS_ABSTRACT_PROJECT_USER_INTERACTOR_H_

#include <sup/gui/project/i_project_user_interactor.h>

#include <QString>

class QWidget;

namespace mvvm
{
struct UserInteractionContext;
}

namespace sup::gui
{

/**
 * @brief The AbstractProjectUserInteractor provides partial implementation of
 * IProjectUserInteractor interface to interact with the user while saving projects.
 *
 * It is responsible for calling correct methods (open file, open dir) while handling paths for
 * file-based and folder-based projects.
 */
class AbstractProjectUserInteractor : public IProjectUserInteractor
{
public:
  explicit AbstractProjectUserInteractor(const QString& application_type, QWidget* parent);

  /**
   * @brief Returns current working directory.
   *
   * Working directory is the parent directory of last opened project.
   */
  std::string GetCurrentWorkdir() const;

  /**
   * @brief Sets the current working directory to a given value (full path).
   */
  void SetCurrentWorkdir(const std::string& path);

  std::string GetNewProjectPath() const override;

  std::string GetExistingProjectPath() const override;

  mvvm::SaveChangesAnswer OnSaveCurrentChangesRequest() const override;

  void SendMessage(const std::string& message) const override;

  /**
   * @brief Sets the flag responsible for using system native file/directory selection dialogs.
   *
   * When true, will use system native dialogs, if the system has one (default). When false, will
   * use Qt's own dialog.
   */
  void SetUseNativeDialog(bool value);

  /**
   * @brief Returns the value if native dialog flag.
   */
  bool GetUseNativeDialogFlag() const;

  /**
   * @brief Creates interaction context with callbacks for use outside of this class.
   */
  mvvm::UserInteractionContext CreateContext() const;

protected:
  /**
   * @brief Updates the current working directory from the project path.
   *
   * If the project path is a path to a file, then the working directory will be the directory where
   * the file is located. If the project path is a path to a folder, then, similarly, the working
   * directory is the parent dir.
   *
   * @param path The path to the project where we save results (full path).
   */
  void UpdateCurrentWorkdir(const std::string& path) const;

  /**
   * @brief Returns application type.
   */
  QString GetApplicationType() const;

  /**
   * @brief Returns parent widget/
   */
  QWidget* GetParent() const;

private:
  /**
   * @brief Provides actual implementation to get new project path.
   */
  virtual std::string GetNewProjectPathImpl() const = 0;

  /**
   * @brief Provides actual implementation to get existing project path.
   */
  virtual std::string GetExistingProjectPathImpl() const = 0;

  //!< The parent directory, from where the user opened the project last time. Made mutable since is
  //!< updated from const methods.
  mutable std::string m_current_workdir;

  //!< Configures the usage of native system file dialog, when true. Alternatively, will use Qt's
  //!< own dialogs.
  bool m_use_native_dialogs{true};

  //!< the type of the application
  QString m_application_type;

  //!< parent to use for modal dialog centering
  QWidget* m_parent{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ABSTRACT_PROJECT_USER_INTERACTOR_H_
