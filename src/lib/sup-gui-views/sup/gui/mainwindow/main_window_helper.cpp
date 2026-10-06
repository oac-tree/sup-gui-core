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

#include "main_window_helper.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/menu_bar_action_registry.h>
#include <sup/gui/core/version_helper.h>
#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/settings/workdir_info.h>
#include <sup/gui/widgets/settings_callbacks.h>

#include <mvvm/utils/file_utils.h>
#include <mvvm/widgets/app_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QLocale>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <iostream>

namespace
{

const QString kPlasmaPreferredStyle = "Breeze";
const QString kCodacStyle = "Adwaita";  // Gnome-like style installed on CODAC machines

/**
 * @brief Returns preferred application style.
 *
 * Will try to favor user choice, if possible. Will fallback to either "breeze" or "adwaita", if
 * user choice is empty, or doesn't exist.
 *
 * @param The preferred style name, as provided by the user from the command line.
 */
std::optional<QString> GetPrefferedStyle(const QString& preferred_style)
{
  const QStringList preferred_styles({preferred_style, kPlasmaPreferredStyle, kCodacStyle});
  const QStringList available_styles = QStyleFactory::keys();

  for (auto element : preferred_styles)
  {
    if (available_styles.contains(element))
    {
      return element;
    }
  }
  return std::nullopt;
}

//! Opens the dialog to select an existing file. Returns empty string if canceled.
QString SelectFileToOpen(const QString& filter, QWidget* parent, const QString& working_dir)
{
  QFileDialog dialog(parent, "Select file to load", working_dir);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(filter);
  const QStringList selected_files =
      (dialog.exec() == QDialog::Accepted) ? dialog.selectedFiles() : QStringList();
  return selected_files.empty() ? QString() : selected_files.at(0);
}

//! Opens the dialog to select a file to save. Returns empty string if canceled.
QString SelectFileToSave(const QString& name_prototype, const QString& filter, QWidget* parent,
                         const QString& working_dir)
{
  return QFileDialog::getSaveFileName(parent, "Save File", working_dir + "/" + name_prototype,
                                      filter);
}

//! Returns the stored working directory, or the home directory if nothing is stored.
sup::gui::WorkdirInfo GetWorkdirInfo(const sup::gui::ISettingsManager& settings_manager,
                                     const std::string& group)
{
  return settings_manager.Get<sup::gui::WorkdirInfo>(
      group, sup::gui::SettingsScope::kApplication,
      sup::gui::WorkdirInfo{QDir::homePath().toStdString()});
}

//! Returns the directory of the file.
QString GetParentDir(const QString& file_name)
{
  return QString::fromStdString(mvvm::utils::GetParentPath(file_name.toStdString()));
}

}  // namespace

namespace sup::gui
{

void InitCoreApplication(const QString& app_name, const QString& version)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName(app_name);
  QCoreApplication::setApplicationVersion(version);
  QCoreApplication::setOrganizationName("coa");

  if (!HasValidAppSettings(version.toStdString()))
  {
    std::cout << "Application's settings file which was found in $HOME/.config/coa folder is too "
                 "old for this GUI version ["
              << version.toStdString() << "], starting from scratch.\n";

    QSettings settings;
    settings.clear();
    settings.setValue(constants::kAppVersion, version);
  }
}

void SetupApplication(std::int32_t font_size_hint, const QString& app_style, bool verbose)
{
  // first changing style, then font
  SetWindowStyle(app_style);
  SetApplicationFont(font_size_hint);
  if (verbose)
  {
    std::cout << mvvm::utils::GetDesktopInfo();
  }
}

void SetWindowStyle(const QString& app_style)
{
  if (auto preferred_style = GetPrefferedStyle(app_style); preferred_style.has_value())
  {
    if (QApplication::style()->objectName().toLower() != preferred_style.value().toLower())
    {
      QApplication::setStyle(QStyleFactory::create(preferred_style.value()));
    }
  }
}

void SetApplicationFont(std::int32_t font_size_hint)
{
  auto original_app_font = QApplication::font();

  if (font_size_hint > 0)
  {
    original_app_font.setPointSize(font_size_hint);
  }
  else
  {
    auto font_in_settings = GetAppFontFromSettings();
    if (font_in_settings.has_value())
    {
      // We can't use font from settings directly, because on CODAC the font obtained from the
      // serialization is not recognized by the QFontDatabase. At least, calculation of its
      // metrics (size of `M`) looks wrong. Thus, we use both size and family to modify original
      // font.
      original_app_font.setPointSize(font_in_settings.value().pointSize());
      original_app_font.setFamily(font_in_settings.value().family());
    }
  }
  QApplication::setFont(original_app_font);
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

void SaveAppFontInSettings(const QFont& font)
{
  QSettings settings;
  settings.setValue(constants::kAppFontSettingName, font);
}

bool HasValidAppSettings(const std::string& app_version)
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

  const QFont font = QFontDialog::getFont(&font_was_changed, QApplication::font(), nullptr);

  if (font_was_changed)
  {
    SaveAppFontInSettings(font);
    QApplication::setFont(font);
  }

  return font_was_changed;
}

QString GetDialogFileNameToOpen(const QString& filter, QWidget* parent, const QString& key)
{
  QString working_dir{};
  if (!key.isEmpty())
  {
    const auto read_func = sup::gui::GetSettingsReadFunc();
    working_dir = read_func(key).toString();
  }

  const QString file_name = SelectFileToOpen(filter, parent, working_dir);

  if (!file_name.isEmpty() && !key.isEmpty())
  {
    const auto write_func = sup::gui::GetSettingsWriteFunc();
    write_func(key, GetParentDir(file_name));
  }

  return file_name;
}

QString GetDialogFileNameToSave(const QString& name_prototype, const QString& filter,
                                QWidget* parent, const QString& key)
{
  QString working_dir{};
  if (!key.isEmpty())
  {
    const auto read_func = sup::gui::GetSettingsReadFunc();
    working_dir = read_func(key).toString();
  }

  const QString file_name = SelectFileToSave(name_prototype, filter, parent, working_dir);

  if (!file_name.isEmpty() && !key.isEmpty())
  {
    const auto write_func = sup::gui::GetSettingsWriteFunc();
    write_func(key, GetParentDir(file_name));
  }

  return file_name;
}

QString GetDialogFileNameToOpen(const QString& filter, QWidget* parent,
                                ISettingsManager& settings_manager, const std::string& group)
{
  const auto workdir = GetWorkdirInfo(settings_manager, group);
  const QString file_name = SelectFileToOpen(filter, parent, QString::fromStdString(workdir.path));

  if (!file_name.isEmpty())
  {
    settings_manager.Set(group, WorkdirInfo{GetParentDir(file_name).toStdString()},
                         SettingsScope::kApplication);
  }

  return file_name;
}

QString GetDialogFileNameToSave(const QString& name_prototype, const QString& filter,
                                QWidget* parent, ISettingsManager& settings_manager,
                                const std::string& group)
{
  const auto workdir = GetWorkdirInfo(settings_manager, group);
  const QString file_name =
      SelectFileToSave(name_prototype, filter, parent, QString::fromStdString(workdir.path));

  if (!file_name.isEmpty())
  {
    settings_manager.Set(group, WorkdirInfo{GetParentDir(file_name).toStdString()},
                         SettingsScope::kApplication);
  }

  return file_name;
}

}  // namespace sup::gui
