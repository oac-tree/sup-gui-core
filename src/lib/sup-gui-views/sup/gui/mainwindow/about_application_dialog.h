/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#ifndef SUP_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
#define SUP_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_

#include <QDialog>
#include <QLabel>
#include <memory>

namespace sup::gui
{

/**
 * @brief The AboutApplicationDialog class is the main dialog
 */
class AboutApplicationDialog : public QDialog
{
  Q_OBJECT
public:
  AboutApplicationDialog(QWidget *parent = nullptr, const QString &app_name = QString(),
                         const QString &description = QString(),
                         const QString &version = QString());
  ~AboutApplicationDialog() override = default;

  static void ShowDialog(QWidget *parent = nullptr, const QString &app_name = QString(),
                         const QString &description = QString(),
                         const QString &version = QString());

  static QString GetCopyRight();

private:
  std::unique_ptr<QLabel> CreateLinkLabel(const QString &link, const QString &name);

  std::unique_ptr<QLayout> CreateLogoLayout();
  std::unique_ptr<QLayout> CreateTextLayout(const QString &app_name, const QString &description,
                                            const QString &version);
  std::unique_ptr<QLayout> CreateButtonLayout();
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
