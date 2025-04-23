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

#include "message_helper.h"

namespace sup::gui
{

void SendMessage(const sup::gui::MessageEvent &event, QMessageBox::Icon icon_type)
{
  QMessageBox msg_box;
  msg_box.setWindowTitle(QString::fromStdString(event.title));
  msg_box.setText(QString::fromStdString(event.text));
  msg_box.setInformativeText(QString::fromStdString(event.informative));
  msg_box.setDetailedText(QString::fromStdString(event.detailed));
  msg_box.setIcon(icon_type);
  msg_box.exec();
}

void SendWarningMessage(const sup::gui::MessageEvent &event)
{
  SendMessage(event, QMessageBox::Warning);
}

void SendInfoMessage(const sup::gui::MessageEvent &event)
{
  SendMessage(event, QMessageBox::Information);
}

}  // namespace sup::gui
