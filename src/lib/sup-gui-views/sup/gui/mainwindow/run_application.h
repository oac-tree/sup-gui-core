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

#include <sup/gui/mainwindow/anyvalue_editor_app_context.h>
#include <sup/gui/mainwindow/command_line_options.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/model/register_items.h>

#include <mvvm/widgets/app_restarter.h>
#include <mvvm/widgets/app_utils.h>

#include <QApplication>

namespace sup::gui
{

/**
 * @brief Create QApplication and main window.
 *
 * @tparam MainWindowT The type of main window to create.
 */
template <typename MainWindowT>
int RunApplication(int argc, char** argv)
{
  mvvm::AppRestarter restarter(argc, argv);

  sup::gui::RegisterSessionItems();

  auto options = ParseOptions(argc, argv);
  mvvm::utils::SetupHighDpiScaling(options.scale);

  const QApplication app(argc, argv);

  SetupApplication(options.system_font_psize, options.style, options.info);

  AnyValueEditorAppContext context;
  MainWindowT win(context);
  if (options.window_size.has_value())
  {
    win.resize(options.window_size.value());
  }
  win.show();

  if (!options.file_name.isEmpty())
  {
    win.ImportAnyValueFromFile(options.file_name);
  }

  return restarter.HandleExit(app.exec());
}

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_RUN_APPLICATION_H_
