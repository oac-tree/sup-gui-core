/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_SUP_DTO_MODEL_H_
#define SUP_GUI_SUP_DTO_MODEL_H_

#include <mvvm/model/application_model.h>
#include <vector>

namespace mvvm
{
class ContainerItem;
}

namespace sup::gui
{

/**
 * @brief The SupDtoModel class is the main model for editing multiple AnyValueItem in the context
 * of AnyValueEditor.
 *
 * AnyValueEditor is intended to edit one AnyValueItem at a time. Each item is located in its own
 * container and AnyValueEditor receives this container as an input.
 */
class SupDtoModel : public mvvm::ApplicationModel
{
public:
  explicit SupDtoModel(std::shared_ptr<mvvm::ItemPool> pool = {});

  void Clear() override;

  std::vector<mvvm::ContainerItem *> GetContainers() const;

private:
  /**
   * @brief Populate the model with the initial content.
   *
   * By default the model contains single container for AnyValueItem.
   */
  void PopulateModel();
};

}  // namespace sup::gui

#endif  // SUP_GUI_SUP_DTO_MODEL_H_
