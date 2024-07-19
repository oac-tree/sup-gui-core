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

#ifndef SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_
#define SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_

//! @file
//! Collection of helper methods common for all sequencer main windows.

#include <sup/gui/views/anyvalueeditor/command_line_options.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/app/main_window_types.h>

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QSettings>

namespace sup::gui
{

/**
 * @brief Create QApplication and main window.
 * @tparam MainWindowT The type of main window to create.
 */

template <typename MainWindowT>
int RunApplication(int argc, char** argv)
{
  auto options = ParseOptions(argc, argv);
  sup::gui::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  const auto default_font = app.font();

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  int exit_code{0};
  std::unique_ptr<MainWindowT> win;

  do
  {
    if (exit_code == sup::gui::CleanSettingsAndRestart)
    {
      QSettings settings;
      settings.clear();
      mvvm::utils::SetApplicationFont(default_font);
    }

    win = std::make_unique<MainWindowT>();
    win->show();

    exit_code = app.exec();
  } while (exit_code != sup::gui::NormalExit);

  return exit_code;
}

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_
