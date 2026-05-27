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

#ifndef SUP_GUI_WIDGETS_FLATLIST_DELEGATE_H_
#define SUP_GUI_WIDGETS_FLATLIST_DELEGATE_H_

#include <QStyledItemDelegate>

namespace sup::gui
{

/**
 * @brief The FlatListDelegate class provides closing icon in a second column of a tree.
 */
class FlatListDelegate : public QStyledItemDelegate
{
public:
  explicit FlatListDelegate(QObject* parent = nullptr);

  void setCloseButtonVisible(bool visible);
  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;

  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
  bool m_close_button_visible{true};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_FLATLIST_DELEGATE_H_
