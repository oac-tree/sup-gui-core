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

#include "main_window.h"

#include <mvvm/widgets/app_utils.h>

#include <QApplication>
#include <QLocale>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName("anyvalue-editor");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setOrganizationName("coa");

  mvvm::utils::SetupHighDpiScaling();

  QApplication app(argc, argv);

  anyvalueeditor::MainWindow win;

  win.show();

  return app.exec();
}
