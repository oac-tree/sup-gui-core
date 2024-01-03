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

#include <sup/gui/anyvalueeditor/anyvalue_editor_main_window.h>
#include <sup/gui/anyvalueeditor/command_line_options.h>
#include <sup/gui/anyvalueeditor/custom_metatypes.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/app/main_window_helper.h>
#include <sup/gui/core/version.h>

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QSettings>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sup::gui::ProjectVersion());

  sup::gui::InitCoreApplication("anyvalue-editor", version);

  auto options = anyvalueeditor::ParseOptions(argc, argv);

  sup::gui::SetupHighDpiScaling(options.scale);

  sup::gui::RegisterCustomMetaTypes();

  QApplication app(argc, argv);

  const auto default_font = app.font();

  sup::gui::SetupApplication(options.system_font_psize, options.style, options.info);

  int exit_code{0};
  std::unique_ptr<anyvalueeditor::AnyValueEditorMainWindow> win;

  do
  {
    if (exit_code == sup::gui::CleanSettingsAndRestart)
    {
      QSettings settings;
      settings.clear();
      mvvm::utils::SetApplicationFont(default_font);
    }

    win = std::make_unique<anyvalueeditor::AnyValueEditorMainWindow>();
    win->show();

    exit_code = app.exec();
  } while (exit_code != sup::gui::NormalExit);

  return exit_code;
}
