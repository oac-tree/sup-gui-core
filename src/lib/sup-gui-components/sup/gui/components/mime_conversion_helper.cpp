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

#include "mime_conversion_helper.h"

#include "item_filter_helper.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/providers/qtcore_helper.h>

#include <QMimeData>

namespace sup::gui
{

QStringList GetStringListFromMime(const QMimeData& mime_data, const QString& mime_type)
{
  if (!mime_data.hasFormat(mime_type))
  {
    return {};
  }
  return mvvm::utils::GetStringList(mime_data.data(mime_type));
}

void SetStringListToMime(const QStringList& string_list, const QString& mime_type,
                         QMimeData& mime_data)
{
  mime_data.setData(mime_type, mvvm::utils::GetByteArray(string_list));
}

std::vector<std::string> GetItemTypesFromMime(const QMimeData& mime_data, const QString& mime_type)
{
  if (!mime_data.hasFormat(mime_type))
  {
    return {};
  }

  const auto string_list = GetStringListFromMime(mime_data, mime_type);
  return mvvm::utils::GetStdStringVector(string_list);
}

void SetItemTypesToMime(const std::vector<const mvvm::SessionItem*>& items,
                        const QString& mime_type, QMimeData& mime_data)
{
  QStringList item_types;
  item_types.reserve(static_cast<QStringList::size_type>(items.size()));
  for (const auto& item : items)
  {
    item_types.push_back(QString::fromStdString(item->GetType()));
  }
  SetStringListToMime(item_types, mime_type, mime_data);
}

std::vector<std::string> GetItemIdentifiersFromMime(const QMimeData& mime_data,
                                                    const QString& mime_type)
{
  if (!mime_data.hasFormat(mime_type))
  {
    return {};
  }

  const auto string_list = GetStringListFromMime(mime_data, mime_type);
  return mvvm::utils::GetStdStringVector(string_list);
}

void SetItemIdentifiersToMime(const std::vector<const mvvm::SessionItem*>& items,
                              const QString& mime_type, QMimeData& mime_data)
{
  QStringList identifiers;
  identifiers.reserve(static_cast<QStringList::size_type>(items.size()));
  for (const auto& item : items)
  {
    identifiers.push_back(QString::fromStdString(item->GetIdentifier()));
  }
  SetStringListToMime(identifiers, mime_type, mime_data);
}

void CopyItemsToMimeData(const std::vector<const mvvm::SessionItem*>& top_level_items,
                         const QString& mime_type, QMimeData& mime_data)
{
  CopyItemsToMimeData(top_level_items, mime_type, mime_data, {});
}

void CopyItemsToMimeData(const std::vector<const mvvm::SessionItem*>& top_level_items,
                         const QString& mime_type, QMimeData& mime_data,
                         const std::function<bool(const mvvm::SessionItem&)>& filter_func)
{
  QStringList xml_representation;
  for (const auto& item : top_level_items)
  {
    xml_representation.push_back(
        QString::fromStdString(mvvm::utils::ToXMLString(*item, filter_func)));
  }
  SetStringListToMime(xml_representation, mime_type, mime_data);
}

std::vector<std::unique_ptr<mvvm::SessionItem> > CreateSessionItemsFromMimeData(
    const QMimeData& mime_data, const QString& mime_format)
{
  if (!mime_data.hasFormat(mime_format))
  {
    return {};
  }

  std::vector<std::unique_ptr<mvvm::SessionItem> > result;

  auto binary_data = mime_data.data(mime_format);
  for (const auto& xml_str : mvvm::utils::GetStringList(binary_data))
  {
    (void)result.emplace_back(mvvm::utils::SessionItemFromXMLString(xml_str.toStdString()));
  }

  return result;
}

std::unique_ptr<QMimeData> CreateCopyMimeData(const std::vector<const mvvm::SessionItem*>& items,
                                              const QString& mime_type)
{
  auto result = std::make_unique<QMimeData>();
  sup::gui::CopyItemsToMimeData(items, mime_type, *result);
  return result;
}

}  // namespace sup::gui
