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

#include "application_helper.h"

#include <mvvm/widgets/app_utils.h>

#include <QApplication>
#include <QLocale>
#include <QProcess>
#include <QStyleFactory>
#include <iostream>

namespace
{
const QString kPreferredCodacStyle = "Adwaita";
}

namespace sup::gui
{

bool IsOnCodac()
{
#ifdef CODAC_FOUND
  return true;
#else
  return false;
#endif
}

void InitCoreApplication(const QString &app_name, const QString &version)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName(app_name);
  QCoreApplication::setApplicationVersion(version);
  QCoreApplication::setOrganizationName("coa");
}

void SetWindowStyle(const QString &app_style, int font_size, bool verbose)
{
  if (!app_style.isEmpty())
  {
    QApplication::setStyle(QStyleFactory::create(app_style));
  }
  else
  {
    if (IsOnCodac())
    {
      // If no special request from the user, and we are on CODAC, use Adwaita style
      // which provide tolerable gnome-like UI
      QApplication::setStyle(QStyleFactory::create(kPreferredCodacStyle));
    }
  }

  mvvm::utils::SetApplicationFontSize(font_size);

  if (verbose)
  {
    std::cout << mvvm::utils::GetDesktopInfo();
  }
}

void SetupHighDpiScaling(bool scale_from_environment)
{
  mvvm::utils::SetupHighDpiScaling(scale_from_environment);
}

QString GetUserName()
{
#ifdef Q_OS_UNIX
  QProcess process;
  process.start("whoami", QStringList(), QIODevice::ReadOnly);
  if (process.waitForFinished(500) && process.exitStatus() == QProcess::NormalExit)
  {
    return QString(process.readAllStandardOutput()).trimmed();
  }
  else
  {
    process.kill();
    return "";
  }
// #elif Q_OS_WIN
// TODO: implement with Win32 API "GetUserName"?
#else
  return "";
#endif
}

}  // namespace sup::gui
