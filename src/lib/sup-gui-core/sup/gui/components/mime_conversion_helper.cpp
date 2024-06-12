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

#include "mime_conversion_helper.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMimeData>

namespace sup::gui
{

std::unique_ptr<QMimeData> CreateCopyMimeData(const mvvm::SessionItem& item,
                                              const QString& mime_format)
{
  auto result = std::make_unique<QMimeData>();
  auto xml_str = mvvm::utils::ToXMLString(item);
  result->setData(mime_format, mvvm::utils::GetByteArray({QString::fromStdString(xml_str)}));
  QString clipboard_text =
      QString("Copy of item '%1'").arg(QString::fromStdString(item.GetDisplayName()));
  result->setText(clipboard_text);
  return result;
}

std::unique_ptr<mvvm::SessionItem> CreateSessionItem(const QMimeData* mime_data,
                                                     const QString& mime_format)
{
  if (!mime_data || !mime_data->hasFormat(mime_format))
  {
    return {};
  }

  auto binary_data = mime_data->data(mime_format);
  auto list = mvvm::utils::GetStringList(binary_data);
  if (!list.empty())
  {
    auto xml_str = list.front().toStdString();
    return mvvm::utils::SessionItemFromXMLString(xml_str);
  }

  return {};
}

}  // namespace sup::gui
