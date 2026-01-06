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

#ifndef SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_
#define SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_

//! @file
//! Collection of helper methods common for all sequencer main windows.

#include <sup/gui/app/main_window_types.h>
#include <sup/gui/components/custom_metatypes.h>
#include <sup/gui/mainwindow/anyvalue_editor_app_context.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/views/anyvalueeditor/command_line_options.h>

#include <mvvm/widgets/app_utils.h>

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
  mvvm::utils::SetupHighDpiScaling(options.scale);

  QApplication app(argc, argv);

  const auto default_font = app.font();

  SetupApplication(options.system_font_psize, options.style, options.info);

  int exit_code{0};

  do
  {
    if (exit_code == CleanSettingsAndRestart)
    {
      QSettings settings;
      settings.clear();
      QApplication::setFont(default_font);
    }

    AnyValueEditorAppContext context;
    MainWindowT win(context);

    if (options.window_size.has_value())
    {
      win.resize(options.window_size.value());
    }
    win.show();

    exit_code = app.exec();
  } while (exit_code != NormalExit);

  return exit_code;
}

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_
