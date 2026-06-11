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

#ifndef SUP_GUI_COMPONENTS_ITEM_DISPLAY_NAME_COMBO_ADAPTER_H_
#define SUP_GUI_COMPONENTS_ITEM_DISPLAY_NAME_COMBO_ADAPTER_H_

#include <QList>
#include <QString>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

/**
 * @brief The ItemDisplayNameComboAdapter class is a helper to convert collection of items to combo
 * box entries.
 *
 * The first entry of QComboBox means "no selection", all other intries contains simple item
 * display names. The adapter allows to convert items to combo indexes and back.
 */
class ItemDisplayNameComboAdapter
{
public:
  static constexpr auto kNoSelection = " < no selection >";

  template <typename ElementT>
  explicit ItemDisplayNameComboAdapter(const std::vector<ElementT>& items);

  /**
   * @brief Get list of item names to be shown in combo toolbar.
   *
   * First item will be "no selection".
   */
  QList<QString> GetAvailableItemNames() const;

  /**
   * @brief Get configuration item corresponding to the currently selected item in combo toolbar.
   */
  mvvm::SessionItem* GetItemFromComboIndex(std::int32_t combo_index);

  /**
   * @brief Get combo toolbar index corresponding to the given configuration item.
   */
  std::int32_t GetComboIndexFromItem(const mvvm::SessionItem* item);

private:
  std::vector<mvvm::SessionItem*> m_items;
};

template <typename ElementT>
inline ItemDisplayNameComboAdapter::ItemDisplayNameComboAdapter(const std::vector<ElementT>& items)
{
  m_items.reserve(items.size());
  for (auto* item : items)
  {
    m_items.push_back(item);
  }
}

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ITEM_DISPLAY_NAME_COMBO_ADAPTER_H_