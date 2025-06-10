/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
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

#include "custom_row_waveform_strategies.h"

#include <mvvm/standarditems/point_item.h>
#include <mvvm/viewmodel/viewitem.h>
#include <mvvm/viewmodel/viewitem_factory.h>

#include <stdexcept>

namespace
{

/**
 * @brief Constructs row of two data items pointing to values of "x" and "y" fields of PointItem.
 */
std::vector<std::unique_ptr<mvvm::ViewItem>> CreateXYRow(const mvvm::PointItem &item)
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  auto scalar_items = item.GetAllItems();
  if (scalar_items.size() != 2)
  {
    throw std::runtime_error("Error in TwoColumRowStrategy: not intended for this type of item");
  }

  (void)result.emplace_back(mvvm::CreateDataViewItem(scalar_items.at(0)));
  (void)result.emplace_back(mvvm::CreateDataViewItem(scalar_items.at(1)));

  return result;
}

}  // namespace

namespace sup::gui
{

// -------------------------------------------------------------------------------------------------
// TwoColumRowStrategy
// -------------------------------------------------------------------------------------------------

std::size_t TwoColumRowStrategy::GetSize() const
{
  return 2U;
}

QStringList TwoColumRowStrategy::GetHorizontalHeaderLabels() const
{
  static const QStringList result = {"X", "Y"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> TwoColumRowStrategy::ConstructRowImpl(
    mvvm::SessionItem *item)
{
  // For structures construct a row made of data's of two fields
  if (auto point_item = dynamic_cast<const mvvm::PointItem *>(item); point_item)
  {
    return CreateXYRow(*point_item);
  }

  return {};
}

}  // namespace sup::gui
