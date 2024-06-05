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

#include "anyvalue_editor_actions.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace sup::gui
{

AnyValueEditorActions::AnyValueEditorActions(QObject *parent)
    : QObject(parent), m_create_anyvalue_menu(CreateAddAnyValueMenu())
{
  SetupActions();
}

AnyValueEditorActions::~AnyValueEditorActions() = default;

QList<QAction *> AnyValueEditorActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void AnyValueEditorActions::SetupActions()
{
  // Add
  m_add_anyvalue_action = new ActionMenu(this);
  m_add_anyvalue_action->setText("Add");
  m_add_anyvalue_action->setIcon(utils::GetIcon("plus-circle-outline.svg"));
  m_add_anyvalue_action->setToolTip(
      "Add new AnyValue to the view. If the view already\n"
      "contains AnyValue, try to add new AnyValue as a\n"
      "field to current selection");
  m_add_anyvalue_action->setMenu(m_create_anyvalue_menu.get());
  m_action_map.Add(ActionKey::kInsertAfter, m_add_anyvalue_action);

  // Remove selected
  m_remove_selected_action = new QAction(this);
  m_remove_selected_action->setText("Remove");
  m_remove_selected_action->setIcon(utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_selected_action->setToolTip("Remove selected item and all it's children");
  connect(m_remove_selected_action, &QAction::triggered, this,
          &AnyValueEditorActions::RemoveSelectedRequest);
  m_action_map.Add(ActionKey::kRemoveSelected, m_remove_selected_action);

  // MoveUp button
  m_move_up_action = new QAction(this);
  m_move_up_action->setText("Move Up");
  m_move_up_action->setIcon(utils::GetIcon("arrow-up-thin-circle-outline.svg"));
  m_move_up_action->setToolTip("Move currently selected field up (works within the same parent)");
  connect(m_move_up_action, &QAction::triggered, this, &AnyValueEditorActions::MoveUpRequest);
  m_action_map.Add(ActionKey::kMoveUp, m_move_up_action);

  // MoveDown button
  m_move_down_action = new QAction(this);
  m_move_down_action->setText("Move Down");
  m_move_down_action->setIcon(utils::GetIcon("arrow-down-thin-circle-outline.svg"));
  m_move_down_action->setToolTip(
      "Move currently selected field down (works within the same parent)");
  connect(m_move_down_action, &QAction::triggered, this, &AnyValueEditorActions::MoveDownRequest);
  m_action_map.Add(ActionKey::kMoveDown, m_move_down_action);
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
