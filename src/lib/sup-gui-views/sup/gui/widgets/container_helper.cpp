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

#include "container_helper.h"

#include <QIODevice>
#include <QByteArray>
#include <QDataStream>

namespace sup::gui
{

QByteArray GetArray(const QList<int> &list)
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream << list;
  return data;
}

QList<int> GetList(const QByteArray &array)
{
  QByteArray sd = array;
  QDataStream stream(&sd, QIODevice::ReadOnly);
  QList<int> result;
  stream >> result;
  return result;
}

}
