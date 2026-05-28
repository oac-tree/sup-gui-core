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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIG_COMBO_ADAPTER_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIG_COMBO_ADAPTER_H_

#include <QList>
#include <QString>
#include <functional>

namespace sup::gui
{

class ConfigurationItem;

/**
 * @brief The ConfigComboAdapter class is a helper to convert ConfigurationItem to combo selector
 * indexes and back.
 *
 * The first entry of ComboSelector means "no selection", so the whole class is about substracting
 * or adding an index.
 */
class ConfigComboAdapter
{
public:
  static constexpr auto kNoSelection = " < no selection >";

  explicit ConfigComboAdapter(const std::vector<ConfigurationItem*>& items);

  /**
   * @brief Get list of item names to be shown in combo toolbar.
   *
   * First item will be "no selection".
   */
  QList<QString> GetAvailableItemNames() const;

  /**
   * @brief Get configuration item corresponding to the currently selected item in combo toolbar.
   */
  const ConfigurationItem* GetItemFromComboIndex(std::int32_t combo_index) const;

  /**
   * @brief Get combo toolbar index corresponding to the given configuration item.
   */
  std::int32_t GetComboIndexFromItem(const ConfigurationItem* item);

private:
  std::vector<ConfigurationItem*> m_items;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIG_COMBO_ADAPTER_H_