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

#include "main_window_helper.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_action_manager.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_helper.h>
#include <sup/gui/core/version_helper.h>

#include <mvvm/widgets/app_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QFontDialog>
#include <QLocale>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QStyleFactory>
#include <iostream>

namespace
{
const QString kPreferredCodacStyle = "Adwaita";
}  // namespace

namespace sup::gui
{

void InitCoreApplication(const QString &app_name, const QString &version)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName(app_name);
  QCoreApplication::setApplicationVersion(version);
  QCoreApplication::setOrganizationName("coa");

  if (!HasValidAppSettings(version.toStdString()))
  {
    std::cout << "Application's settings file which was found in $HOME/.config/coa folder is too "
                 "old for this GUI version ["
              << version.toStdString() << "], starting from scratch." << std::endl;

    QSettings settings;
    settings.clear();
    settings.setValue(constants::kAppVersion, version);
  }
}

void SetupHighDpiScaling(bool scale_from_environment)
{
  mvvm::utils::SetupHighDpiScaling(scale_from_environment);
}

void SetupApplication(std::int32_t font_size_hint, const QString &app_style, bool verbose)
{
  SetApplicationFont(font_size_hint);
  SetWindowStyle(app_style);
  if (verbose)
  {
    std::cout << mvvm::utils::GetDesktopInfo();
  }
}

void ShutdownApplication()
{
  // asking global ActionManager to forget about all registered toolbars
  GetGlobalActionManager().Reset();
}

void SetWindowStyle(const QString &app_style)
{
  if (!app_style.isEmpty())
  {
    QApplication::setStyle(QStyleFactory::create(app_style));
  }
  else
  {
    if (sup::gui::IsOnCodac())
    {
      // If no special request from the user, and we are on CODAC, use Adwaita style
      // which provide tolerable gnome-like UI
      QApplication::setStyle(QStyleFactory::create(kPreferredCodacStyle));
    }
  }
}

void SetApplicationFont(std::int32_t font_size_hint)
{
  if (font_size_hint > 0)
  {
    mvvm::utils::SetApplicationFontSize(font_size_hint);
  }
  else
  {
    auto font = GetAppFontFromSettings();
    if (font.has_value())
    {
      QApplication::setFont(font.value());
    }
  }
}

QString GetUserName()
{
#ifdef Q_OS_UNIX
  const std::int32_t wait_msec{500};
  QProcess process;
  process.start("whoami", QStringList(), QIODevice::ReadOnly);
  if (process.waitForFinished(wait_msec) && process.exitStatus() == QProcess::NormalExit)
  {
    return QString(process.readAllStandardOutput()).trimmed();
  }
  process.kill();
  return {};

// #elif Q_OS_WIN
// TODO: implement with Win32 API "GetUserName"?
#else
  return {};
#endif
}

std::optional<QFont> GetAppFontFromSettings()
{
  const QSettings settings;

  if (settings.contains(constants::kAppFontSettingName))
  {
    return settings.value(constants::kAppFontSettingName).value<QFont>();
  }

  return {};
}

void SaveAppFontInSettings(const QFont &font)
{
  QSettings settings;
  settings.setValue(constants::kAppFontSettingName, font);
}

bool HasValidAppSettings(const std::string &app_version)
{
  const QSettings settings;

  if (settings.contains(constants::kAppVersion))
  {
    auto settings_version = settings.value(constants::kAppVersion).toString().toStdString();

    return HasSameMajorMinorVersion(app_version, settings_version);
  }

  return false;
}

bool ShouldResetSettingsAndRestart()
{
  const QString question_text =
      "All persistent application settings (i.e. window sizes, position of splitters, etc) "
      " will be reset to their default values. Restart is required.";

  QMessageBox msgBox;
  msgBox.setText(question_text);
  msgBox.setInformativeText("Do you want to reset settings and restart application?\n");

  auto yes_button = msgBox.addButton("Yes, please restart", QMessageBox::YesRole);
  msgBox.addButton("Cancel", QMessageBox::NoRole);

  msgBox.exec();
  return msgBox.clickedButton() == yes_button;
}

bool SummonChangeSystemFontDialog()
{
  bool font_was_changed{false};

  QFont font = QFontDialog::getFont(&font_was_changed, QApplication::font(), nullptr);

  if (font_was_changed)
  {
    sup::gui::SaveAppFontInSettings(font);
    mvvm::utils::SetApplicationFont(font);
  }

  return font_was_changed;
}

bool IsHeadlessMode()
{
  if (const char* var = std::getenv("QT_QPA_PLATFORM"); var != nullptr)
  {
    return std::string(var) == std::string("offscreen");
  }
  return false;
}

}  // namespace sup::gui
