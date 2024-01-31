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

#include <sup/gui/codeeditor/code_view.h>
#include <sup/gui/components/visibility_agent_base.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/project/model_has_changed_controller.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

#include <QCheckBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sup::gui
{

AnyValueEditorTextPanel::AnyValueEditorTextPanel(mvvm::SessionModelInterface *model,
                                                 QWidget *parent)
    : QWidget(parent), m_json_view(new CodeView(CodeView::kJSON)), m_model(model)
{
  setWindowTitle("JSON view");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_json_view);

  SetupActions();

  auto on_subscribe = [this]() { SetupController(); };
  auto on_unsubscribe = [this]()
  {
    m_model_changed_controller.reset();
    m_json_view->ClearText();
  };
  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

void AnyValueEditorTextPanel::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  m_container = container;
}

void AnyValueEditorTextPanel::SetJSONPretty(bool value)
{
  if (m_pretty_json != value)
  {
    m_pretty_json = value;
    m_pretty_button->setIcon(m_pretty_json ? utils::GetIcon("checkbox-marked-circle-outline")
                                           : utils::GetIcon("checkbox-blank-circle-outline"));
    UpdateJson();
  }
}

void AnyValueEditorTextPanel::SetupActions()
{
  {
    m_pretty_button = new QToolButton;
    m_pretty_button->setText("Pretty");
    m_pretty_button->setIcon(utils::GetIcon("checkbox-marked-circle-outline"));
    m_pretty_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pretty_button->setToolTip("Make JSON pretty");

    auto on_action = [this]() { SetJSONPretty(!m_pretty_json); };
    connect(m_pretty_button, &QToolButton::clicked, this, on_action);

    m_pretty_action = new QWidgetAction(this);
    m_pretty_action->setDefaultWidget(m_pretty_button);
    addAction(m_pretty_action);
  }

  {
    auto button = new QToolButton;
    button->setText("Export");
    button->setIcon(utils::GetIcon("file-export-outline"));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setToolTip("Export AnyValue to JSON file");
    connect(button, &QToolButton::clicked, this, &AnyValueEditorTextPanel::ExportToFileRequest);
    m_export_action = new QWidgetAction(this);
    m_export_action->setDefaultWidget(button);
    addAction(m_export_action);
  }
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

void AnyValueEditorTextPanel::UpdateJson()
{
  if (!m_container || m_container->GetTotalItemCount() == 0)
  {
    return;
  }

  if (auto item = m_container->GetItem<AnyValueItem>(mvvm::TagIndex::Default(0)); item)
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

void AnyValueEditorTextPanel::SetupController()
{
  auto on_model_changed = [this]() { UpdateJson(); };
  m_model_changed_controller =
      std::make_unique<mvvm::ModelHasChangedController>(m_model, on_model_changed);
  UpdateJson();
}

}  // namespace sup::gui
