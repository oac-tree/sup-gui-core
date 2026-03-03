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

#ifndef SUP_GUI_COMPONENTS_MIME_CONVERSION_HELPER_H_
#define SUP_GUI_COMPONENTS_MIME_CONVERSION_HELPER_H_

//! @file
//! Helper methods to convert item from/to QMimeData.

#include <QString>
#include <functional>
#include <memory>
#include <vector>

class QMimeData;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

//! mime type to copy-and-paste AnyValue
constexpr auto kCopyAnyValueMimeType = "application/coa.sequencer.anyvalue.copy";

/**
 * @brief Returns type of item enclosed inside the given mime data.
 *
 * Will return empty string if mime has wrong format.
 *
 * @param mime_data The mime data.
 * @param mime_format Expected format in mime data.
 */
std::string GetSessionItemType(const QMimeData* mime_data, const QString& mime_format);

/**
 * @brief Creates mime data to copy given item.
 *
 * @param item The item to copy.
 * @param mime_format String representing format.
 * @param filter_func Predicate to filter out some children.
 */
std::unique_ptr<QMimeData> CreateCopyMimeData(
    const mvvm::SessionItem& item, const QString& mime_format,
    const std::function<bool(const mvvm::SessionItem&)>& filter_func = {});

/**
 * @brief Creates mime data to copy given list of items.
 *
 * This functions packs given list of items into a single mime data. It is expected that items are
 * not in parent-child relationship, otherwise the resulting mime data will contain duplicates.
 *
 * @param item Items to copy.
 * @param mime_format String representing format.
 * @param filter_func Predicate to filter out some children.
 */
std::unique_ptr<QMimeData> CreateCopyMimeData(
    const std::vector<const mvvm::SessionItem*>& items, const QString& mime_format,
    const std::function<bool(const mvvm::SessionItem&)>& filter_func = {});

/**
 * @brief Creates item from given mime data.
 *
 * @param mime_data Mime data from copy operation.
 * @param mime_format String representing format.
 * @return New item.
 */
std::unique_ptr<mvvm::SessionItem> CreateSessionItem(const QMimeData* mime_data,
                                                     const QString& mime_format);

/**
 * @brief Creates items from the given mime data.
 *
 * It is expected that mime data contains vector of items.
 *
 * @param mime_data Mime data from copy operation.
 * @param mime_format String representing format.
 * @return Item list.
 */
std::vector<std::unique_ptr<mvvm::SessionItem>> CreateSessionItems(const QMimeData* mime_data,
                                                                   const QString& mime_format);

/**
 * @brief Creates mime data to copy given list of items.
 *
 * If parent is selected, all its children will be also copied. The item list will be first filtered
 * to exclude children, if their parents are already present in the list to avoid duplicates.
 *
 * The method is used in copy-and-paste, and drag-and-drop scenario, when the user manually picks
 * parent and child items from a large tree.
 *
 * @param item Items to copy.
 * @param mime_format String representing format.
 *
 * @see GetTopLevelSelection()
 */
std::unique_ptr<QMimeData> CreateItemSelectionCopyMimeData(
    const std::vector<const mvvm::SessionItem*>& items, const QString& mime_format);

/**
 * @brief Adds item selection to the given mime data.
 *
 * @param items Items to copy.
 * @param mime_format String representing format.
 * @param mime_data Mime data to add information to.
 *
 * @see CreateItemSelectionCopyMimeData()
 */
void AddItemSelectionCopyToMimeData(const std::vector<const mvvm::SessionItem*>& items,
                                    const QString& mime_format, QMimeData& mime_data);


}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_MIME_CONVERSION_HELPER_H_
