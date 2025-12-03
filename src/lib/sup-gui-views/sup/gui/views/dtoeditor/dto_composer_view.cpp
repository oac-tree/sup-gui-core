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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "dto_composer_view.h"

#include "dto_composer_actions.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/component_objects/dto_composer_action_handler.h>
#include <sup/gui/component_objects/dto_composer_tab_controller.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

#include <QMenu>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

DtoComposerTabController::create_widget_callback_t CreateCallback(
    IAppCommandService& command_service)
{
  return [&command_service](mvvm::SessionItem* item) -> std::unique_ptr<AnyValueEditorWidget>
  {
    auto result = std::make_unique<AnyValueEditorWidget>(command_service);
    result->SetAnyValueItemContainer(item);
    return result;
  };
}

}  // namespace

DtoComposerView::DtoComposerView(IAppCommandService& command_service, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_tab_widget(new QTabWidget)
    , m_actions(new DtoComposerActions(this))
    , m_action_handler(new DtoComposerActionHandler(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_tab_widget);

  m_tab_widget->setMovable(true);
  m_tab_widget->setTabsClosable(true);

  m_tab_widget->setTabPosition(QTabWidget::South);
  m_tab_widget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tab_widget->tabBar(), &QTabBar::customContextMenuRequested, this,
          &DtoComposerView::SummonContextMenu);

  for (auto action : m_actions->GetActions())
  {
    m_command_service.AddActionToMenuBar(sup::gui::constants::kToolsMenu, action);
  }

  SetupConnections();
}

DtoComposerView::~DtoComposerView() = default;

void DtoComposerView::SetModel(mvvm::ISessionModel* model)
{
  m_action_handler->SetModel(model);
  m_tab_controller = std::make_unique<DtoComposerTabController>(
      model, CreateCallback(m_command_service), m_tab_widget);
}

void DtoComposerView::SetupConnections()
{
  // Connecting DtoComposerActions requests (add/duplicate/remove tabs) with
  // DtoComposerActionHandler. Please note, that we do not add/remove tabs manually. The
  // DtoComposerActionHandler will add/remove underlying container on every add/remove tab request.
  // This in turn will be noticed by DtoComposerActionController, which will do actual tab insertion
  // and remove.

  // the request to add a new tab
  connect(m_actions, &DtoComposerActions::AddNewTabRequest, m_action_handler,
          &DtoComposerActionHandler::OnAddNewContainer);

  // the request to duplicate existing tab
  connect(m_actions, &DtoComposerActions::DuplicateCurrentTabRequest, m_action_handler,
          [this]() { m_action_handler->OnDuplicateContainer(m_tab_widget->currentIndex()); });

  // the request to remove current tab
  connect(m_actions, &DtoComposerActions::RemoveCurrentTabRequest, m_action_handler,
          [this]() { m_action_handler->OnRemoveContainer(m_tab_widget->currentIndex()); });

  // close button on tab corner
  connect(m_tab_widget->tabBar(), &QTabBar::tabCloseRequested, m_action_handler,
          &DtoComposerActionHandler::OnRemoveContainer);
}

void DtoComposerView::SummonContextMenu(const QPoint& point)
{
  QMenu menu;
  menu.setToolTipsVisible(true);

  menu.addActions(m_actions->GetActions());

  menu.exec(m_tab_widget->tabBar()->mapToGlobal(point));
}

}  // namespace sup::gui
