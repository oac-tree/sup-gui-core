/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <sup/gui/components/custom_metatypes.h>
#include <sup/gui/core/version.h>
#include <sup/gui/mainwindow/dto_editor_main_window.h>
#include <sup/gui/mainwindow/run_application.h>
#include <sup/gui/model/register_items.h>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sup::gui::ProjectVersion());
  sup::gui::InitCoreApplication("sup-dto-editor", version);

  sup::gui::RegisterCustomMetaTypes();
  sup::gui::RegisterSessionItems();

  return sup::gui::RunApplication<sup::gui::DtoEditorMainWindow>(argc, argv);
}
