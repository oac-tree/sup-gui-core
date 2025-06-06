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

#include "container_helper.h"

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

namespace sup::gui
{

QByteArray GetByteArrayFromList(const QList<int> &list)
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream << list;
  return data;
}

QList<int> GetListFromByteArray(const QByteArray &array)
{
  QByteArray sd = array;
  QDataStream stream(&sd, QIODevice::ReadOnly);
  QList<int> result;
  stream >> result;
  return result;
}

}  // namespace sup::gui
