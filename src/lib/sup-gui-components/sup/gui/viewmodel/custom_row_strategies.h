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

#ifndef SUP_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_
#define SUP_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_

#include <mvvm/viewmodel/abstract_row_strategy.h>

namespace sup::gui
{

/**
 * @brief The AnyValueRowStrategy class is a strategy to generate row of items representing
 * AnyValueItem tree.
 */
class AnyValueRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_
