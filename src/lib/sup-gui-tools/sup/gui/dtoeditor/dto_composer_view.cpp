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

#include "dto_composer_view.h"

#include <QTabWidget>
#include <QVBoxLayout>

namespace sup::gui
{

DtoComposerView::DtoComposerView(QWidget *parent) : QWidget(parent), m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_tab_widget);

  m_tab_widget->addTab(new QWidget, "AnyValue");

  m_tab_widget->setTabPosition(QTabWidget::South);
}

}  // namespace sup::gui
