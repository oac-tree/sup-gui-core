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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
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
const QString kCopyAnyValueMimeType = "application/coa.sequencer.anyvalue.copy";

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

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_MIME_CONVERSION_HELPER_H_
