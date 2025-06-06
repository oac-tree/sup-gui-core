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

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

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

  auto on_subscribe = [this]() { SetupListener(); };
  auto on_unsubscribe = [this]()
  {
    m_listener.reset();
    m_json_view->ClearText();
  };
  // will be deleted as a child of QObject
  m_visibility_agent = new VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

void AnyValueEditorTextPanel::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  m_container = container;
  if (isVisible())
  {
    SetupListener();
  }
}

void AnyValueEditorTextPanel::SetJSONPretty(bool value)
{
  if (m_pretty_json != value)
  {
    m_pretty_json = value;
    m_pretty_action->setIcon(m_pretty_json ? utils::FindIcon("checkbox-marked-circle-outline")
                                           : utils::FindIcon("checkbox-blank-circle-outline"));
    UpdateJson();
  }
}

mvvm::ISessionModel *AnyValueEditorTextPanel::GetModel()
{
  return m_container ? m_container->GetModel() : nullptr;
}

void AnyValueEditorTextPanel::SendMessage(const std::string &what) const
{
  const std::string title("JSON generation failed");
  const std::string text("The AnyValue being edited is in inconsistent state");
  MessageEvent message{title, text, what, ""};
  m_message_handler->SendMessage(message);
}

void AnyValueEditorTextPanel::SetupActions()
{
  m_pretty_action = new QAction(this);
  m_pretty_action->setText("Pretty");
  m_pretty_action->setIcon(utils::FindIcon("checkbox-marked-circle-outline"));
  m_pretty_action->setToolTip("Make JSON pretty");
  auto on_action = [this]() { SetJSONPretty(!m_pretty_json); };
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

void AnyValueEditorTextPanel::UpdateJson()
{
  if (auto item = GetAnyValueItem(); item)
  {
    try
    {
      // Current simplified approach calls the method `UpdateJson` on every
      // model change. If model is inconsistent, CreateAnyValue method will fail.

      auto any_value = sup::gui::CreateAnyValue(*item);
      auto str = sup::gui::AnyValueToJSONString(any_value, m_pretty_json);
      m_json_view->SetContent(QString::fromStdString(str));
    }
    catch (const std::exception &ex)
    {
      m_json_view->ClearText();
      SendMessage(ex.what());
    }
  }
  else
  {
    m_json_view->ClearText();
  }
}

void AnyValueEditorTextPanel::SetupListener()
{
  if (GetModel())
  {
    m_listener = std::make_unique<mvvm::ModelListener>(GetModel());

    m_listener->Connect<mvvm::ModelAboutToBeResetEvent>([this](auto)
                                                        { SetAnyValueItemContainer(nullptr); });

    m_listener->Connect<mvvm::DataChangedEvent>([this](auto) { UpdateJson(); });
    m_listener->Connect<mvvm::ItemInsertedEvent>([this](auto) { UpdateJson(); });
    m_listener->Connect<mvvm::ItemRemovedEvent>([this](auto) { UpdateJson(); });

    UpdateJson();
  }
}

AnyValueItem *AnyValueEditorTextPanel::GetAnyValueItem()
{
  if (!m_container || m_container->GetTotalItemCount() == 0)
  {
    return nullptr;
  }

  return m_container->GetItem<AnyValueItem>(mvvm::TagIndex::Default(0));
}

}  // namespace sup::gui
