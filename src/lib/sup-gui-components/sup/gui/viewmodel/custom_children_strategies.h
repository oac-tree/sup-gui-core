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

#ifndef SUP_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
#define SUP_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_

#include <mvvm/viewmodel/i_children_strategy.h>

namespace sup::gui
{

/**
 * @brief The AnyValueChildrenStrategy class is a strategy to children related to AnyValueItem.
 */
class AnyValueChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(const mvvm::SessionItem* item) const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
