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

#include "mime_conversion_helper.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/qtcore_helper.h>

#include <QMimeData>

namespace sup::gui
{

std::string GetSessionItemType(const QMimeData* mime_data, const QString& mime_format)
{
  if (!mime_data || !mime_data->hasFormat(mime_format))
  {
    return {};
  }

  auto item = sup::gui::CreateSessionItem(mime_data, mime_format);
  return item ? item->GetType() : std::string();
}

std::unique_ptr<QMimeData> CreateCopyMimeData(
    const mvvm::SessionItem& item, const QString& mime_format,
    const std::function<bool(const mvvm::SessionItem&)>& filter_func)
{
  return CreateCopyMimeData({&item}, mime_format, filter_func);
}

std::unique_ptr<QMimeData> CreateCopyMimeData(
    const std::vector<const mvvm::SessionItem*>& items, const QString& mime_format,
    const std::function<bool(const mvvm::SessionItem&)>& filter_func)
{
  if (items.empty())
  {
    return {};
  }

  auto result = std::make_unique<QMimeData>();
  QStringList xml_representation;
  QString clipboard_text("Copy of item");
  for (auto item : items)
  {
    xml_representation.push_back(
        QString::fromStdString(mvvm::utils::ToXMLString(*item, filter_func)));
    (void) clipboard_text.append(" " + QString::fromStdString(item->GetDisplayName()));
  }
  result->setData(mime_format, mvvm::utils::GetByteArray(xml_representation));
  result->setText(clipboard_text);
  return result;
}

std::unique_ptr<mvvm::SessionItem> CreateSessionItem(const QMimeData* mime_data,
                                                     const QString& mime_format)
{
  auto items = CreateSessionItems(mime_data, mime_format);
  return items.empty() ? std::unique_ptr<mvvm::SessionItem>() : std::move(items.front());
}

std::vector<std::unique_ptr<mvvm::SessionItem> > CreateSessionItems(const QMimeData* mime_data,
                                                                    const QString& mime_format)
{
  std::vector<std::unique_ptr<mvvm::SessionItem> > result;

  if (!mime_data || !mime_data->hasFormat(mime_format))
  {
    return {};
  }

  auto binary_data = mime_data->data(mime_format);
  for (const auto& xml_str : mvvm::utils::GetStringList(binary_data))
  {
    (void) result.emplace_back(mvvm::utils::SessionItemFromXMLString(xml_str.toStdString()));
  }

  return result;
}

}  // namespace sup::gui
