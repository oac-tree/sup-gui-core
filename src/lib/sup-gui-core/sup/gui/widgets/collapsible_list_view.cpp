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

#include "collapsible_list_view.h"

#include "collapsible_toolbar.h"
#include "collapsible_widget.h"

#include <sup/gui/core/exceptions.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

CollapsibleListView::CollapsibleListView(QWidget *parent)
    : QWidget(parent), m_splitter(new QSplitter)
{
  m_splitter->setOrientation(Qt::Vertical);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_splitter);
}

//! Adds widget to the splitter.
//! Widget is added as it is, without collapsible bar and menus.
void CollapsibleListView::AddWidget(QWidget *content)
{
  m_splitter->addWidget(content);
}

CollapsibleToolBar *CollapsibleListView::AddCollapsibleWidget(QWidget *content,
                                                              const QList<QAction *> &actions)
{
  if (!content)
  {
    throw RuntimeException("CollapsibleListView: can't add a null widget");
  }
  auto collapsible_widget = new CollapsibleWidget(content);
  collapsible_widget->AddToSplitter(m_splitter);

  // To show user actions in a tool bar.
  for (auto action : actions)
  {
    collapsible_widget->GetToolBar()->AddAction(action);
  }

  return collapsible_widget->GetToolBar();
}

QSplitter *CollapsibleListView::GetSplitter() const
{
  return m_splitter;
}

}  // namespace sup::gui
