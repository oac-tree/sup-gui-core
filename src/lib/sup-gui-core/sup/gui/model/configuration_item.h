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

#ifndef SUP_GUI_MODEL_CONFIGURATION_ITEM_H_
#define SUP_GUI_MODEL_CONFIGURATION_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief The ConfigurationItem class holds a configuration and auxiliary information.
 *
 * Holds a single instance of AnyValueItem representing a configuration.
 * Might hold more data in the future (i.e. backup values, reference to other items to compare,
 * etc).
 */
class ConfigurationItem : public mvvm::CompoundItem
{
public:
  ConfigurationItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns main configuration item.
   */
  AnyValueItem* GetConfiguration();

  /**
   * @brief Returns file name from which the configuration was initially imported.
   */
  std::string GetFileName() const;

  /**
   * @brief Sets file name from which the configuration was imported.
   */
  void SetFileName(const std::string& value);
};

}  // namespace sup::gui

namespace mvvm
{

template <>
struct item_traits<sup::gui::ConfigurationItem>
{
  static constexpr std::string_view type_name() noexcept { return "ConfigurationItem"; }
};

}  // namespace mvvm

#endif  // SUP_GUI_MODEL_CONFIGURATION_CONTAINER_ITEM_H_
