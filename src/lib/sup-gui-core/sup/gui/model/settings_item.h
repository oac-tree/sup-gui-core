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

#ifndef SUP_GUI_MODEL_SETTINGS_ITEM_H_
#define SUP_GUI_MODEL_SETTINGS_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{
class PropertyChangedEvent;
}

namespace sup::gui
{

/**
 * @brief The CommonSettingsItem class represents a group of individual application settings.
 *
 * It's a compound item, where each property corresponds to an individual application settings.
 */
class CommonSettingsItem : public mvvm::CompoundItem
{
public:
  CommonSettingsItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

private:
  /**
   * @brief Custom data strategy to enable/disabled status of "undo limits" property when
   * "undo flag" changes.
   */
  bool OnSetFlag(SessionItem* property, const mvvm::variant_t& variant, mvvm::role_t role);
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_SETTINGS_ITEM_H_
