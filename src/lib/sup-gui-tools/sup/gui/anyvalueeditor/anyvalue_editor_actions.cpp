/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_actions.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace sup::gui
{

AnyValueEditorActions::AnyValueEditorActions(QObject *parent)
    : QObject(parent), m_create_anyvalue_menu(CreateAddAnyValueMenu())
{
  SetupActions();
}

AnyValueEditorActions::~AnyValueEditorActions() = default;

QList<QAction *> AnyValueEditorActions::GetActions() const
{
  return {m_add_anyvalue_action, m_remove_selected_action};
}

void AnyValueEditorActions::SetupActions()
{
  // QAction with menu doesn't provide instant popup capabilities. Let's use QToolButton and wrap it
  // into QWidgetAction.

  {  // add button
    auto button = new QToolButton;
    button->setText("Add");
    button->setIcon(utils::GetIcon("plus-circle-outline"));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setToolTip(
        "Add new AnyValue to the view. If the view already\n"
        "contains AnyValue, try to add new AnyValue as a\n"
        "field to current selection");
    button->setPopupMode(QToolButton::InstantPopup);
    button->setMenu(m_create_anyvalue_menu.get());
    m_add_anyvalue_action = new QWidgetAction(this);
    m_add_anyvalue_action->setDefaultWidget(button);
  }

  {  // remove button
    auto button = new QToolButton;
    button->setText("Remove");
    button->setIcon(utils::GetIcon("beaker-remove-outline"));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setToolTip("Remove selected item and all it's children");
    connect(button, &QToolButton::clicked, this, &AnyValueEditorActions::RemoveSelectedRequest);
    m_remove_selected_action = new QWidgetAction(this);
    m_remove_selected_action->setDefaultWidget(button);
  }
}

std::unique_ptr<QMenu> AnyValueEditorActions::CreateAddAnyValueMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  {  // empty
    auto action = result->addAction("empty");
    connect(action, &QAction::triggered, this, &AnyValueEditorActions::AddEmptyAnyValueRequest);
  }

  {  // struct
    auto action = result->addAction("struct");
    connect(action, &QAction::triggered, this, &AnyValueEditorActions::AddAnyValueStructRequest);
  }

  {  // array
    auto action = result->addAction("array");
    connect(action, &QAction::triggered, this, &AnyValueEditorActions::AddAnyValueArrayRequest);
  }

  {
    auto scalar_menu = result->addMenu("scalar");
    for (const auto &name : sup::gui::GetScalarTypeNames())
    {
      auto str = QString::fromStdString(name);
      auto on_action = [str, this]() { emit AddAnyValueScalarRequest(str); };
      auto action = scalar_menu->addAction(str);
      connect(action, &QAction::triggered, this, on_action);
    }
  }

  result->addSeparator();

  auto action = result->addAction("Import from file");
  connect(action, &QAction::triggered, this, &AnyValueEditorActions::ImportFromFileRequest);

  return result;
}

}  // namespace sup::gui
