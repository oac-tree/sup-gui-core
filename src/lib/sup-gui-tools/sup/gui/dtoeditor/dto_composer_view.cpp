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

#include "dto_composer_action_handler.h"
#include "dto_composer_actions.h"
#include "dto_composer_tab_controller.h"

#include <sup/gui/app/app_action_helper.h>

#include <QTabWidget>
#include <QVBoxLayout>

namespace sup::gui
{

DtoComposerView::DtoComposerView(mvvm::SessionModelInterface *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_tab_widget(new QTabWidget)
    , m_tab_controller(std::make_unique<DtoComposerTabController>(model, m_tab_widget))
    , m_actions(new DtoComposerActions(this))
    , m_action_handler(new DtoComposerActionHandler(m_model, this))
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_tab_widget);

  m_tab_widget->setMovable(true);
  m_tab_widget->setTabsClosable(true);

  m_tab_widget->setTabPosition(QTabWidget::South);

  // Actions for main window's tools menu
  sup::gui::AppRegisterActions(sup::gui::constants::kToolsMenu, m_actions->GetActions());

  SetupConnections();
}

void DtoComposerView::SetupConnections()
{
  // The request to remove a tab will remove AnyValueItem container. This in turn will be noticed by
  // DtoComposerTabController which will remove the tab itself.
  connect(m_actions, &DtoComposerActions::RemoveCurrentTabRequest, m_action_handler,
          [this]() { m_action_handler->OnRemoveContainer(m_tab_widget->currentIndex()); });

  // The request to add a new tab will add a new AnyValueItem container. This in turn will be
  // noticed by DtoComposerTabController which will add actual tab.
  connect(m_actions, &DtoComposerActions::AddNewTabRequest, m_action_handler,
          &DtoComposerActionHandler::OnAddNewContainer);
}

}  // namespace sup::gui
