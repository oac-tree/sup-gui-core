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

#include "dto_composer_tab_controller.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/standarditems/container_item.h>

#include <QTabWidget>
#include <QVBoxLayout>

namespace sup::gui
{

DtoComposerView::DtoComposerView(mvvm::SessionModelInterface *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_tab_widget(new QTabWidget)
    , m_tab_controller(std::make_unique<DtoComposerTabController>(model, m_tab_widget))
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_tab_widget);

  m_tab_widget->setMovable(true);
  m_tab_widget->setTabsClosable(true);

  m_tab_widget->setTabPosition(QTabWidget::South);

  AddAnyValue();
}

void DtoComposerView::AddAnyValue()
{
  m_model->InsertItem<mvvm::ContainerItem>();
}

}  // namespace sup::gui
