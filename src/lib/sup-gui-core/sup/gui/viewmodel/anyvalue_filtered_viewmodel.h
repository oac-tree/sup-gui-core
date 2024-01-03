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

#ifndef SUP_GUI_VIEWMODEL_ANYVALUE_FILTERED_VIEWMODEL_H_
#define SUP_GUI_VIEWMODEL_ANYVALUE_FILTERED_VIEWMODEL_H_

#include <QSortFilterProxyModel>

namespace sup::gui
{

class AnyValueFilteredViewModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit AnyValueFilteredViewModel(QObject* parent = nullptr);

  void SetPattern(const QString& pattern);

  bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
  QString m_pattern;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWMODEL_ANYVALUE_FILTERED_VIEWMODEL_H_
