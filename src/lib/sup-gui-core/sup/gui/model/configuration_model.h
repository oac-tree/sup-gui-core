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

#ifndef SUP_GUI_MODEL_CONFIGURATION_MODEL_H_
#define SUP_GUI_MODEL_CONFIGURATION_MODEL_H_

#include <mvvm/model/application_model.h>

namespace sup::gui
{

class ConfigurationContainerItem;

/**
 * @brief The ConfigurationModel class holds AnyValue-based configurations.
 *
 * For the moment it is used to compare configurations in DtoCompareView.
 */
class ConfigurationModel : public mvvm::ApplicationModel
{
public:
  explicit ConfigurationModel(std::shared_ptr<mvvm::ItemPool> pool = {});

  void Clear() override;

  ConfigurationContainerItem* GetConfigurationContainer();

private:
  /**
   * @brief Populate the model with the initial content.
   */
  void PopulateModel();

  ConfigurationContainerItem* m_configuration_container{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_CONFIGURATION_MODEL_H_