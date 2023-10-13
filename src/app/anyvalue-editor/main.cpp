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

#include "anyvalue_editor_main_window.h"
#include "command_line_options.h"

#include <sup/gui/core/version.h>
#include <sup/gui/widgets/application_helper.h>
#include <sup/gui/anyvalueeditor/custom_metatypes.h>

#include <QApplication>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sup::gui::ProjectVersion());

  sup::gui::InitCoreApplication("anyvalue-editor", version);

  auto options = anyvalueeditor::ParseOptions(argc, argv);

  sup::gui::SetupHighDpiScaling(options.scale);

  sup::gui::RegisterCustomMetaTypes();

  QApplication app(argc, argv);

  sup::gui::SetWindowStyle(options.style, options.system_font_psize, options.info);

  anyvalueeditor::AnyValueEditorMainWindow win;

  win.show();

  return app.exec();
}
