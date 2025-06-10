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

#include "anyvalue_editor_textpanel.h"

#include <sup/gui/components/json_panel_controller.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/standarditems/container_item.h>

#include <QAction>
#include <QCheckBox>
#include <QToolButton>
#include <QVBoxLayout>

namespace sup::gui
{

AnyValueEditorTextPanel::AnyValueEditorTextPanel(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_json_view(new CodeView(CodeView::kJSON))
    , m_message_handler(sup::gui::CreateWidgetOverlayMessageHandler(m_json_view))
{
  setWindowTitle("JSON view");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_json_view);

  SetupActions();

  auto on_subscribe = [this]() { SetContainerIntern(m_container); };
  auto on_unsubscribe = [this]() { SetContainerIntern(nullptr); };
  // will be deleted as a child of QObject
  m_visibility_agent = new VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

void AnyValueEditorTextPanel::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  if (m_container == container)
  {
    return;
  }

  m_container = container;

  if (container && isVisible())
  {
    SetContainerIntern(m_container);
  }
}

void AnyValueEditorTextPanel::SetJsonPretty(bool value)
{
  m_panel_controller->SetPrettyJson(value);
  m_pretty_action->setIcon(value ? utils::FindIcon("checkbox-marked-circle-outline")
                                 : utils::FindIcon("checkbox-blank-circle-outline"));
}

void AnyValueEditorTextPanel::SetContainerIntern(mvvm::SessionItem *container)
{
  if (container)
  {
    auto on_json_update = [this](const auto &xml)
    {
      m_message_handler->ClearMessages();
      m_json_view->SetContent(QString::fromStdString(xml));
    };

    auto on_message = [this](const auto &message)
    {
      m_json_view->ClearText();
      m_message_handler->SendMessage(message);
    };

    m_panel_controller =
        std::make_unique<JsonPanelController>(container, on_json_update, on_message);
    m_panel_controller->SetPrettyJson(true);
  }
  else
  {
    m_panel_controller.reset();
    m_json_view->ClearText();
  }
}

void AnyValueEditorTextPanel::SendMessage(const std::string &what) const
{
  const std::string title("JSON generation failed");
  const std::string text("The AnyValue being edited is in inconsistent state");
  const MessageEvent message{title, text, what, ""};
  m_message_handler->SendMessage(message);
}

void AnyValueEditorTextPanel::SetupActions()
{
  m_pretty_action = new QAction(this);
  m_pretty_action->setText("Pretty");
  m_pretty_action->setIcon(utils::FindIcon("checkbox-marked-circle-outline"));
  m_pretty_action->setToolTip("Make JSON pretty");
  auto on_action = [this]() { SetJsonPretty(!m_panel_controller->IsPrettyJson()); };
  connect(m_pretty_action, &QAction::triggered, this, on_action);
  addAction(m_pretty_action);

  m_export_action = new QAction(this);
  m_export_action->setText("Export");
  m_export_action->setIcon(utils::FindIcon("file-export-outline"));
  m_export_action->setToolTip("Export AnyValue to JSON file");
  connect(m_export_action, &QAction::triggered, this,
          &AnyValueEditorTextPanel::ExportToFileRequest);
  addAction(m_export_action);
}

}  // namespace sup::gui
