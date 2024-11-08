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

#include "dialog_helper.h"

#include <mvvm/widgets/widget_utils.h>

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace sup::gui
{

std::unique_ptr<QVBoxLayout> CreateButtonLayout(QDialog* dialog, const QString& ok_text,
                                                const QString& cancel_text)
{
  auto result = std::make_unique<QVBoxLayout>();

  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton(ok_text, QDialogButtonBox::AcceptRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  button = button_box->addButton(cancel_text, QDialogButtonBox::RejectRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  QDialogButtonBox::connect(button_box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  QDialogButtonBox::connect(button_box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

  const auto gap = mvvm::utils::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  result->addWidget(button_box);
  return result;
}

}  // namespace sup::gui
