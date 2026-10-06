/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "anyvalue_editor.h"

#include "anyvalue_editor_widget.h"

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <QVBoxLayout>

namespace
{

const bool kEnableUndo = true;
const std::size_t kUndoLimit = 100;

/**
 * @brief Creates the editor widget, storing its settings in the manager if given, or in QSettings.
 */
std::unique_ptr<sup::gui::AnyValueEditorWidget> CreateEditorWidget(
    sup::gui::IAppCommandService& command_service, sup::gui::ISettingsManager* settings_manager)
{
  if (settings_manager != nullptr)
  {
    return std::make_unique<sup::gui::AnyValueEditorWidget>(
        sup::gui::AppViewContext{command_service, *settings_manager});
  }
  return std::make_unique<sup::gui::AnyValueEditorWidget>(command_service);
}

}  // namespace

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget* parent_widget) : AnyValueEditor(nullptr, parent_widget) {}

AnyValueEditor::AnyValueEditor(ISettingsManager& settings_manager, QWidget* parent_widget)
    : AnyValueEditor(&settings_manager, parent_widget)
{
}

AnyValueEditor::AnyValueEditor(ISettingsManager* settings_manager, QWidget* parent_widget)
    : AbstractAnyValueEditor(parent_widget)
    , m_command_service(std::make_unique<NullCommandService>())
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_editor_widget(CreateEditorWidget(*m_command_service, settings_manager))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_editor_widget.get());

  m_model->SetUndoEnabled(kEnableUndo, kUndoLimit);
  m_editor_widget->SetAnyValueItemContainer(m_model->GetRootItem());
}

AnyValueEditor::~AnyValueEditor() = default;

void AnyValueEditor::SetInitialValue(const AnyValueItem* item)
{
  if (item == nullptr)
  {
    return;
  }
  m_editor_widget->SetInitialValue(*item);
}

std::unique_ptr<AnyValueItem> AnyValueEditor::GetResult() const
{
  return m_editor_widget->GetResult();
}

const AnyValueItem* AnyValueEditor::GetTopItem() const
{
  return m_editor_widget->GetTopItem();
}

}  // namespace sup::gui
