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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <memory>

class QAction;
class QMenu;

namespace sup::gui
{

class ActionMenu;
class AnyValueEditorActionHandler;

/**
 * @brief The AnyValueEditorActions class contains collection of actions to edit AnyValue tree.
 */
class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey
  {
    kInsertAfter,
    kInsertInto,
    kRemoveSelected,
    kMoveUp,
    kMoveDown
  };

  explicit AnyValueEditorActions(AnyValueEditorActionHandler* action_handler,
                                 QObject* parent = nullptr);
  ~AnyValueEditorActions() override;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

signals:
  void ImportFromFileRequest();
  void RemoveSelectedRequest();
  void MoveUpRequest();
  void MoveDownRequest();

private:
  void SetupActions();

  /**
   * @brief Creates a menu intended for insert actions.
   */
  std::unique_ptr<QMenu> CreateInsertMenu();

  /**
   * @brief Populates menu with actions to insert AnyValue items.
   */
  void AboutToShowInsertMenu();

  /**
   * @brief Adds actions to insert AnyValueItem into the given menu.
   *
   * Depending on the value of the "insert_into" flag, actions will emit either "insert into" or
   * "inser after" requests.
   *
   * @param names List of AnyValueItem type names to append to the menu.
   * @param menu The menu to populate.
   * @param insert_into Flag denoting the type of the action.
   */
  void AddInsertActions(const std::vector<std::string>& names, QMenu* menu, bool insert_into);

  std::unique_ptr<QMenu> m_insert_after_menu;
  std::unique_ptr<QMenu> m_insert_into_menu;

  AnyValueEditorActionHandler* m_action_handler{nullptr};

  ActionMenu* m_insert_after_action{nullptr};
  ActionMenu* m_insert_into_action{nullptr};
  QAction* m_remove_selected_action{nullptr};
  QAction* m_move_up_action{nullptr};
  QAction* m_move_down_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
