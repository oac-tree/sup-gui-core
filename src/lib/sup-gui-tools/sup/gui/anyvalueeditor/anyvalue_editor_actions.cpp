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

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/components/anyvalue_editor_action_handler.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace
{

/**
 * @brief Sets menu enabled, if at least one of children actions is enabled.
 */
void SetEnabledToChildren(QMenu &menu)
{
  for (auto action : menu.actions())
  {
    if (action->isEnabled())
    {
      menu.setEnabled(true);
      return;
    }
  }
  menu.setEnabled(false);
}
}  // namespace

namespace sup::gui
{

AnyValueEditorActions::AnyValueEditorActions(AnyValueEditorActionHandler *action_handler,
                                             QObject *parent)
    : QObject(parent)
    , m_action_handler(action_handler)
    , m_insert_after_menu(CreateInsertMenu())
    , m_insert_into_menu(CreateInsertMenu())
{
  SetupInsertRemoveActions();
  SetupCutCopyPasteActions();
}

AnyValueEditorActions::~AnyValueEditorActions() = default;

QList<QAction *> AnyValueEditorActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void AnyValueEditorActions::SetupMenu(QMenu &menu)
{
  menu.addAction(m_insert_after_action);
  menu.addAction(m_insert_into_action);

  menu.addAction(m_remove_selected_action);
  m_remove_selected_action->setEnabled(m_action_handler->CanRemove());

  menu.addSeparator();

  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_after_action);
  menu.addAction(m_paste_into_action);
  m_cut_action->setEnabled(m_action_handler->CanCut());
  m_copy_action->setEnabled(m_action_handler->CanCopy());
  m_paste_into_action->setEnabled(m_action_handler->CanPasteInto());
  m_paste_after_action->setEnabled(m_action_handler->CanPasteAfter());
  menu.addSeparator();
}

void AnyValueEditorActions::RegisterActionsForContext(const AppContext &context)
{
  AppAddActionToCommand(m_cut_action, constants::kCutCommandId, context);
  AppAddActionToCommand(m_copy_action, constants::kCopyCommandId, context);
  AppAddActionToCommand(m_paste_after_action, constants::kPasteCommandId, context);
  AppAddActionToCommand(m_paste_into_action, constants::kPasteSpecialCommandId, context);
}

void AnyValueEditorActions::SetupInsertRemoveActions()
{
  // insert after
  m_insert_after_action = new ActionMenu(this);
  m_insert_after_action->setText("Add");
  m_insert_after_action->setIcon(utils::GetIcon("plus-circle-outline.svg"));
  m_insert_after_action->setToolTip(
      "Add a new AnyValue after the current selection.\n\n"
      "Used to create a new AnyValue if the view is empty,\n"
      "to add a new field to a structure, or to add a new\n"
      "element to the array.\n");
  m_insert_after_action->setMenu(m_insert_after_menu.get());
  m_action_map.Add(ActionKey::kInsertAfter, m_insert_after_action);

  // insert into
  m_insert_into_action = new ActionMenu(this);
  m_insert_into_action->setText("Insert");
  m_insert_into_action->setIcon(utils::GetIcon("plus-circle-multiple-outline.svg"));
  m_insert_into_action->setToolTip(
      "Insert new AnyValue into the current selection.\n\n"
      "Used to append a new field to the structure or an element to the array");
  m_insert_into_action->setMenu(m_insert_into_menu.get());
  m_action_map.Add(ActionKey::kInsertInto, m_insert_into_action);

  // Remove selected
  m_remove_selected_action = new QAction(this);
  m_remove_selected_action->setText("Remove");
  m_remove_selected_action->setIcon(utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_selected_action->setToolTip("Remove selected item and all it's children");
  connect(m_remove_selected_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnRemoveSelected(); });
  m_action_map.Add(ActionKey::kRemoveSelected, m_remove_selected_action);

  // MoveUp button
  m_move_up_action = new QAction(this);
  m_move_up_action->setText("Move Up");
  m_move_up_action->setIcon(utils::GetIcon("arrow-up-thin-circle-outline.svg"));
  m_move_up_action->setToolTip("Move currently selected field up (works within the same parent)");
  connect(m_move_up_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnMoveUpRequest(); });
  m_action_map.Add(ActionKey::kMoveUp, m_move_up_action);

  // MoveDown button
  m_move_down_action = new QAction(this);
  m_move_down_action->setText("Move Down");
  m_move_down_action->setIcon(utils::GetIcon("arrow-down-thin-circle-outline.svg"));
  m_move_down_action->setToolTip(
      "Move currently selected field down (works within the same parent)");
  connect(m_move_down_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnMoveDownRequest(); });
  m_action_map.Add(ActionKey::kMoveDown, m_move_down_action);
}

void AnyValueEditorActions::SetupCutCopyPasteActions()
{
  m_cut_action = new QAction(this);
  m_cut_action->setText("Cut");
  m_cut_action->setToolTip("Cuts selected instruction");
  m_cut_action->setShortcut(QKeySequence("Ctrl+X"));
  connect(m_cut_action, &QAction::triggered, this, [this]() { m_action_handler->Cut(); });

  m_copy_action = new QAction(this);
  m_copy_action->setText("Copy");
  m_copy_action->setToolTip("Copies selected instruction");
  m_copy_action->setShortcut(QKeySequence("Ctrl+C"));
  connect(m_copy_action, &QAction::triggered, this, [this]() { m_action_handler->Copy(); });
  m_action_map.Add(ActionKey::kCopy, m_copy_action);

  m_paste_after_action = new QAction(this);
  m_paste_after_action->setText("Paste After");
  m_paste_after_action->setToolTip("Paste selected instruction after current selection");
  m_paste_after_action->setShortcut(QKeySequence("Ctrl+V"));
  connect(m_paste_after_action, &QAction::triggered, this,
          [this]() { m_action_handler->PasteAfter(); });
  m_action_map.Add(ActionKey::kPasteAfter, m_paste_after_action);

  m_paste_into_action = new QAction(this);
  m_paste_into_action->setText("Paste Into");
  m_paste_into_action->setToolTip("Paste selected instruction into current selection");
  m_paste_into_action->setShortcut(QKeySequence("Ctrl+Shift+V"));
  connect(m_paste_into_action, &QAction::triggered, this,
          [this]() { m_action_handler->PasteInto(); });
  m_action_map.Add(ActionKey::kPasteInto, m_paste_into_action);
}

std::unique_ptr<QMenu> AnyValueEditorActions::CreateInsertMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &AnyValueEditorActions::AboutToShowInsertMenu);
  return result;
}

void AnyValueEditorActions::AboutToShowInsertMenu()
{
  auto menu = qobject_cast<QMenu *>(sender());
  const bool insert_into = (menu == m_insert_into_menu.get());

  menu->clear();

  // main action (add empty value, struct, and array)
  const std::vector<std::string> main_types = {
      constants::kEmptyTypeName, constants::kStructTypeName, constants::kArrayTypeName};
  AddInsertActions(main_types, menu, insert_into);

  // scalar menu with scalar actions
  auto scalar_menu = menu->addMenu("scalar");
  AddInsertActions(sup::gui::GetScalarTypeNames(), scalar_menu, insert_into);
  SetEnabledToChildren(*scalar_menu);

  menu->addSeparator();

  auto action = menu->addAction("Import from file");
  connect(action, &QAction::triggered, this, &AnyValueEditorActions::ImportFromFileRequest);
}

void AnyValueEditorActions::AddInsertActions(const std::vector<std::string> &names, QMenu *menu,
                                             bool insert_into)
{
  for (const auto &name : names)
  {
    auto action = menu->addAction(QString::fromStdString(name));
    action->setEnabled(insert_into ? m_action_handler->CanInsertInto(name)
                                   : m_action_handler->CanInsertAfter(name));
    if (insert_into)
    {
      connect(action, &QAction::triggered, this,
              [this, name]() { m_action_handler->OnInsertAnyValueItemInto(name); });
    }
    else
    {
      connect(action, &QAction::triggered, this,
              [this, name]() { m_action_handler->OnInsertAnyValueItemAfter(name); });
    }
  }
}

}  // namespace sup::gui
