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

#ifndef SUP_GUI_DTOEDITOR_MAIN_WINDOW_H_
#define SUP_GUI_DTOEDITOR_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>

#include <QMainWindow>

class QCloseEvent;

namespace dtoeditor
{

/**
 * @brief The AnyValueEditorMainWindow class represents a main window of sup-config-editor
 * application.
 */

class DtoEditorMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  DtoEditorMainWindow();
  ~DtoEditorMainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void InitComponents();
  void ReadSettings();
  void WriteSettings();

  /**
   * @brief Prepare for application shutdown.
   *
   * @return True if application is ready to be closed.
   *
   * @details This perform saving of unsaved projects, writing persistent application settings and
   * stopping possible running jobs.
   */
  bool PrepareForShutdown();
  void OnRestartRequest(sup::gui::AppExitCode exit_code);
};

}  // namespace dtoeditor

#endif  // SUP_GUI_DTOEDITOR_MAIN_WINDOW_H_
