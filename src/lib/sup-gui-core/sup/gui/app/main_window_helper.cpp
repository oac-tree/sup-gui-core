/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "main_window_helper.h"

#include "application_helper.h"

#include <QApplication>
#include <QFontDialog>
#include <QMessageBox>
#include <QPushButton>

namespace sup::gui
{

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

void SummonChangeSystemFontDialog()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, QApplication::font(), nullptr);
  if (ok)
  {
    // the user clicked OK and font is set to the font the user selected
    sup::gui::SaveAppFontInSettings(font);

    QApplication::setFont(font);
    // the problem is static variable in mvvm::FindSizeOfLetterM, it's not enough to restart the
    // window, the whole application should be restarted

    QMessageBox::information(nullptr, "Restart is required",
                             "Please restart application to fully apply changes");
  }
}

}  // namespace sup::gui
