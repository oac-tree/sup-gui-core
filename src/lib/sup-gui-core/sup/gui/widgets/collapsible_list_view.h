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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{

class CollapsibleToolBar;

//! Vertical widget with column of panels displayed one under another.
//! Each panel contains user widget and can be collapsed/expanded. When expanded,
//! the place occupied by the panel can be changed by draging a splitter.

class CollapsibleListView : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleListView(QWidget* parent = nullptr);

  void AddWidget(QWidget* content);

  CollapsibleToolBar* AddCollapsibleWidget(QWidget* content, const QList<QAction*>& actions);

  QSplitter* GetSplitter() const;

private:
  QSplitter* m_splitter{nullptr};
};

}  // namespace mvvm

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
