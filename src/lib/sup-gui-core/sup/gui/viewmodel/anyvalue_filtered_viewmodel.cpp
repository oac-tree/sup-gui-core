/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_filtered_viewmodel.h"

namespace sup::gui
{
AnyValueFilteredViewModel::AnyValueFilteredViewModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void AnyValueFilteredViewModel::SetPattern(const QString &pattern)
{
  m_pattern = pattern;
  invalidateFilter();
}

bool AnyValueFilteredViewModel::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
{
  const QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

  return sourceModel()->data(index0).toString().contains(m_pattern);
}

}  // namespace sup::gui
