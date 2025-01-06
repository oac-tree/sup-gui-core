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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_WIDGETS_CONTAINER_HELPER_H_
#define SUP_GUI_WIDGETS_CONTAINER_HELPER_H_

#include <QByteArray>
#include <QList>
#include <QVariant>

namespace sup::gui
{

/**
 * @brief Converts list to byte array.
 */
QByteArray GetByteArrayFromList(const QList<int> &list);

/**
 * @brief Converts byte array to list.
 */
QList<int> GetListFromByteArray(const QByteArray &array);

/**
 * @brief Returns QVariantList from list of objects.

 * This is to be able to keep QList<int> in QSettings file in human-readable format.
 *
 * @tparam T The type of the object must be supported by Qt's variant system.
 */
template <typename T>
QVariant GetVariantFromList(const QList<T> &list)
{
  QVariantList variantList;
  variantList.reserve(list.size());
  for (const auto &v : list)
  {
    variantList.append(v);
  }
  return variantList;
}

/**
 * @brief Returns list of objects from QVariantList.
 *
 * This is to be able to keep QList<int> in QSettings file in human-readable format.
 *
 * @tparam T The type of the object must be supported by Qt's variant system.
 * @param variant_list The variant containing QVariant list.
 */
template <typename T>
QList<T> GetListFromVariant(const QVariant &variant_list)
{
  QList<T> result;
  foreach (QVariant variant, variant_list.value<QVariantList>())
  {
    result << variant.value<T>();
  }
  return result;
}

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CONTAINER_HELPER_H_
