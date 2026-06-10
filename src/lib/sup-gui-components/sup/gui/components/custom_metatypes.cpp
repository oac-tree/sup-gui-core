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

#include "custom_metatypes.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/item_selection.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/widgets/main_window_types.h>

#include <QDataStream>
#include <mutex>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <cstdint>
#endif

namespace sup::gui
{

void RegisterCustomMetaTypes()
{
  static std::once_flag register_once_flag;
  auto register_func = []()
  {
    (void)qRegisterMetaType<const mvvm::SessionItem*>("const mvvm::SessionItem*");
    (void)qRegisterMetaType<const mvvm::LineSeriesItem*>("const mvvm::LineSeriesItem*");
    (void)qRegisterMetaType<const mvvm::PointItem*>("const mvvm::PointItem*");
    (void)qRegisterMetaType<QList<int>>("QList<int>");
    (void)qRegisterMetaType<mvvm::AppExitRequest>("mvvm::AppExitRequest");
    (void)qRegisterMetaType<mvvm::ItemSelection>("mvvm::ItemSelection");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");
    (void)qRegisterMetaType<std::int32_t>("std::int32_t");
#endif
  };

  std::call_once(register_once_flag, register_func);
}

}  // namespace sup::gui
