/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#ifndef SUP_GUI_WIDGETS_ABOUT_APPLICATION_DIALOG_H_
#define SUP_GUI_WIDGETS_ABOUT_APPLICATION_DIALOG_H_

#include <QDialog>
#include <QLabel>

namespace sup::gui
{

//! About application dialog.

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
  QLabel *CreateLinkLabel(const QString &link, const QString &name);

  QLayout *CreateLogoLayout();
  QLayout *CreateTextLayout(const QString &app_name, const QString &description,
                            const QString &version);
  QLayout *CreateButtonLayout();
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_ABOUT_APPLICATION_DIALOG_H_
