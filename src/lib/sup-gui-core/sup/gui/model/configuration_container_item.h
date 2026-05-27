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

#ifndef SUP_GUI_MODEL_CONFIGURATION_CONTAINER_ITEM_H_
#define SUP_GUI_MODEL_CONFIGURATION_CONTAINER_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{

class ConfigurationItem;

/**
 * @brief The ConfigurationContainerItem class is a container to hold multiple configurations.
 */
class ConfigurationContainerItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kChildrenTag = "kChildrenTag";
  ConfigurationContainerItem();

  std::unique_ptr<SessionItem> Clone() const override;

  std::vector<ConfigurationItem*> GetConfigurations();

  std::size_t GetConfigurationCount() const;
};

}  // namespace sup::gui

namespace mvvm
{

template <>
struct item_traits<sup::gui::ConfigurationContainerItem>
{
  static constexpr std::string_view type_name() noexcept { return "ConfigurationContainerItem"; }
};

}  // namespace mvvm

#endif  // SUP_GUI_MODEL_CONFIGURATION_CONTAINER_ITEM_H_
