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

#include "anyvalue_editor_textpanel.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/style_utils.h>
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

AnyValueEditorTextPanel::AnyValueEditorTextPanel(mvvm::ISessionModel *model, QWidget *parent)
    : QWidget(parent)
    , m_json_view(new CodeView(CodeView::kJSON))
    , m_model(model)
    , m_listener(std::make_unique<listener_t>(m_model))
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
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

void AnyValueEditorTextPanel::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  m_container = container;
  UpdateJson();
}

void AnyValueEditorTextPanel::SetJSONPretty(bool value)
{
  if (m_pretty_json != value)
  {
    m_pretty_json = value;
    m_pretty_action->setIcon(m_pretty_json ? utils::GetIcon("checkbox-marked-circle-outline")
                                           : utils::GetIcon("checkbox-blank-circle-outline"));
    UpdateJson();
  }
}

void AnyValueEditorTextPanel::SetupActions()
{
  m_pretty_action = new QAction(this);
  m_pretty_action->setText("Pretty");
  m_pretty_action->setIcon(utils::GetIcon("checkbox-marked-circle-outline"));
  m_pretty_action->setToolTip("Make JSON pretty");
  auto on_action = [this]() { SetJSONPretty(!m_pretty_json); };
  connect(m_pretty_action, &QAction::triggered, this, on_action);
  addAction(m_pretty_action);

  m_export_action = new QAction(this);
  m_export_action->setText("Export");
  m_export_action->setIcon(utils::GetIcon("file-export-outline"));
  m_export_action->setToolTip("Export AnyValue to JSON file");
  connect(m_export_action, &QAction::triggered, this,
          &AnyValueEditorTextPanel::ExportToFileRequest);
  addAction(m_export_action);
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

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
    }
  }
  else
  {
    m_json_view->ClearText();
  }
}

void AnyValueEditorTextPanel::SetupListener()
{
  m_listener = std::make_unique<listener_t>(m_model);

  m_listener->Connect<mvvm::ModelAboutToBeResetEvent>([this](auto)
                                                      { SetAnyValueItemContainer(nullptr); });

  m_listener->Connect<mvvm::DataChangedEvent>([this](auto) { UpdateJson(); });
  m_listener->Connect<mvvm::ItemInsertedEvent>([this](auto) { UpdateJson(); });
  m_listener->Connect<mvvm::ItemRemovedEvent>([this](auto) { UpdateJson(); });

  UpdateJson();
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
