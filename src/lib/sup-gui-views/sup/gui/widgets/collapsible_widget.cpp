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

#include "sup/gui/widgets/collapsible_widget.h"

#include <sup/gui/widgets/collapsible_toolbar.h>

#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

CollapsibleWidget::CollapsibleWidget(QWidget *content, const QList<QAction *> &actions,
                                     QWidget *parent_widget)
    : QWidget(parent_widget), m_tool_bar(new CollapsibleToolBar)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(content);

  // Please note, that a tool bar doesn't belong to this layout. It will go to the splitter
  // separately.

  m_tool_bar->SetControlledWidget(this);
  m_tool_bar->SetText(content->windowTitle(), content->toolTip());
  m_tool_bar->AddActions(actions);
}

CollapsibleWidget::~CollapsibleWidget() = default;

void CollapsibleWidget::AddToSplitter(QSplitter *splitter)
{
  // we add to the splitter two widgets: a tool bar and this panel
  splitter->addWidget(m_tool_bar);
  splitter->setCollapsible(splitter->indexOf(m_tool_bar), false);
  splitter->addWidget(this);
  splitter->setCollapsible(splitter->indexOf(this), false);
}

void CollapsibleWidget::SetExpanded(bool value)
{
  m_tool_bar->SetExpanded(value);
}

CollapsibleToolBar *CollapsibleWidget::GetToolBar()
{
  return m_tool_bar;
}

}  // namespace sup::gui
