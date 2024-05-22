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

#ifndef SUP_GUI_COMPONENTS_FILE_BASED_USER_INTERACTOR_H_
#define SUP_GUI_COMPONENTS_FILE_BASED_USER_INTERACTOR_H_

#include <sup/gui/components/abstract_project_user_interactor.h>

#include <QString>
#include <string>

class QWidget;

namespace sup::gui
{

/**
 * @brief The FileBasedUserInteractor class is intended for file-based projects and it provides
 * save/discard/cancel and similar dialogs on user request.
 */
class FileBasedUserInteractor : public AbstractProjectUserInteractor
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param application_type The type of application to inform the userm if necessary.
   * @param parent The widget which will be used to center modal dialogs.
   */
  explicit FileBasedUserInteractor(const QString& application_type, QWidget* parent);

private:
  std::string GetNewProjectPathImpl() const override;

  std::string GetExistingProjectPathImpl() const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_FILE_BASED_USER_INTERACTOR_H_
