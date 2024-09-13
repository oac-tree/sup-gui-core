/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
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

#include "main_window_helper.h"

#include "application_helper.h"

#include <mvvm/utils/string_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QDebug>
#include <QFontDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSize>
#include <QString>

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

std::optional<QSize> ParseSizeString(const QString &text)
{
  auto parts = mvvm::utils::SplitString(text.toStdString(), "x");

  if (parts.size() != 2)
  {
    return {};
  }

  auto x = mvvm::utils::StringToInteger(parts.at(0));
  auto y = mvvm::utils::StringToInteger(parts.at(1));

  return x.has_value() && y.has_value() ? QSize(x.value(), y.value()) : std::optional<QSize>{};
}

}  // namespace sup::gui
