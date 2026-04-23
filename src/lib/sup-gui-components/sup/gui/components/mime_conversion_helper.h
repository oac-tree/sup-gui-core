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
 * @brief Returns list of strings from the given mime data.
 *
 * @param mime_data Mime data carrying the list of strings.
 * @param mime_type Expected mime type of the data (i.e. key in data map).
 */
QStringList GetStringListFromMime(const QMimeData& mime_data, const QString& mime_type);

/**
 * @brief Sets the list of strings to the given mime data.
 *
 * @param string_list List of strings to set.
 * @param mime_type Mime type to set in the data.
 * @param mime_data Mime data to set the list of strings to.
 */
void GetStringListToMime(const QStringList& string_list, const QString& mime_type,
                         QMimeData& mime_data);

/**
 * @brief Returns type of item enclosed inside the given mime data.
 *
 * FIXME: remove in the favor of GetItemTypesFromMime later.
 *
 * Will return empty string if mime has wrong format.
 *
 * @param mime_type The mime data.
 * @param mime_format Expected format in mime data.
 */
std::string GetSessionItemType(const QMimeData& mime_data, const QString& mime_type);

/**
 * @brief Returns list of item types enclosed inside the given mime data.
 *
 * Function assumes that item types are stored in mime data as list of simple strings. Will return
 * empty list if mime has wrong format.
 *
 * Functions is used in various copy-and-paste, drag-and-move scenarios, when we want to know
 * which are model types of top-level items selection. For example, it is used to determine whether
 * we can accept dragged item in the drop target, without reconstructing the whole item tree from
 * mime data, which can be expensive.
 *
 * @param mime_data The mime data.
 * @param mime_type Expected mime type of the data (i.e. key in data map).
 */
std::vector<std::string> GetItemTypesFromMime(const QMimeData& mime_data, const QString& mime_type);

/**
 * @brief Sets list of item types to the given mime data.
 *
 * Function is used in various copy-and-paste, drag-and-move scenarios, when we want to know
 * which are model types of top-level items selection. For example, it is used to determine whether
 * we can accept dragged item in the drop target, without reconstructing the whole item tree from
 * mime data, which can be expensive. If it is the use case, it is the responsibility of the user
 * to make sure that items contains top-level selection.
 *
 * @param items List of items to get types from and set to mime data.
 * @param mime_type Mime type to set in the data.
 * @param mime_data Mime data to set the list of item types to.
 */
void SetItemTypesToMime(const std::vector<const mvvm::SessionItem*>& items,
                        const QString& mime_type, QMimeData& mime_data);










/**
 * @brief Creates mime data to copy given item.
 *
 * By default it will create copy of the whole item tree, but it is possible to provide filter
 * function to exclude some children from the copy. Thus, if filter function exists and returns
 * false for some item, the child will be excluded from the copy. Note that filter function will be
 * called for all items in the tree, including the root item.
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
std::unique_ptr<mvvm::SessionItem> CreateSessionItem(const QMimeData& mime_data,
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
std::vector<std::unique_ptr<mvvm::SessionItem>> CreateSessionItems(const QMimeData& mime_data,
                                                                   const QString& mime_format);

/**
 * @brief Adds item selection to the given mime data.
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
 * @see CreateItemSelectionCopyMimeData()
 */
void AddItemSelectionCopyToMimeData(const std::vector<const mvvm::SessionItem*>& items,
                                    const QString& mime_format, QMimeData& mime_data);

/**
 * @brief Creates mime data to copy given list of items.
 *
 * @see GetTopLevelSelection()
 */
std::unique_ptr<QMimeData> CreateItemSelectionCopyMimeData(
    const std::vector<const mvvm::SessionItem*>& items, const QString& mime_format);

/**
 * @brief Adds the list of item's identifiers to mime data.
 *
 * The item list will be first filtered to exclude children, if their parents are already present in
 * the list to avoid duplicates.
 *
 * The method is used in the context of internal tree move, when the user manually picks parent and
 * child items and moves them in another place
 *
 * @param item Items to copy.
 * @param mime_format String representing format.
 */
void AddItemSelectionIdentifierToMimeData(const std::vector<const mvvm::SessionItem*>& items,
                                          const QString& mime_format, QMimeData& mime_data);

/**
 * @brief Creates mime data containing the list of item's identifiers.
 *
 * @see AddItemSelectionIdentifierToMimeData()
 */
std::unique_ptr<QMimeData> CreateItemSelectionIdentifierMimeData(
    const std::vector<const mvvm::SessionItem*>& items, const QString& mime_format);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_MIME_CONVERSION_HELPER_H_
