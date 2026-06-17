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

#include <sup/gui/component_metatypes.h>
#include <sup/gui/core/version.h>
#include <sup/gui/mainwindow/anyvalue_editor_main_window.h>
#include <sup/gui/mainwindow/run_application.h>

int main(int argc, char** argv)
{
  sup::gui::components::RegisterCustomMetaTypes();  // before InitCoreApplication for QSettings

  const auto version = QString::fromStdString(sup::gui::ProjectVersion());
  sup::gui::InitCoreApplication("anyvalue-editor", version);

  return sup::gui::RunApplication<sup::gui::AnyValueEditorMainWindow>(argc, argv);
}
