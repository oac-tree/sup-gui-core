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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <memory>

class QAction;
class QMenu;

namespace sup::gui
{

class ActionMenu;
class AnyValueEditorActionHandler;
class AppContext;
class ProxyAction;

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
    kCut,
    kCopy,
    kPasteAfter,
    kPasteInto,
    kMoveUp,
    kMoveDown,
    kUndo,
    kRedo
  };

  explicit AnyValueEditorActions(AnyValueEditorActionHandler* action_handler,
                                 QObject* parent_object = nullptr);
  ~AnyValueEditorActions() override;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Setup insert/remove, and cut/copy/paste actions in the given menu.
   */
  void SetupMenu(QMenu& menu);

  /**
   * @brief Registers actions for given context.
   *
   * The context is normally provided by the parent widget.
   */
  void RegisterActionsForContext(const sup::gui::AppContext& context);

  /**
   * @brief Update enabled/disabled status of all actions.
   *
   * The method should be called every time we feel that the situation in a tree has changed, and
   * some actions might not be available anymore. The simplest, probably, is to call it every time
   * the user selects some other item in a tree.
   */
  void UpdateEnabledStatus();

signals:
  void ImportFromFileRequest();

private:
  /**
   * @brief Setups all actions related to insert/remove operations.
   */
  void SetupInsertRemoveActions();

  /**
   * @brief Setups all copy-paste actions.
   */
  void SetupCutCopyPasteActions();

  /**
   * @brief Setups undo/erdo actions.
   */
  void SetupUndoRedoActions();

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

  AnyValueEditorActionHandler* m_action_handler{nullptr};

  std::unique_ptr<QMenu> m_insert_after_menu;
  std::unique_ptr<QMenu> m_insert_into_menu;

  ActionMenu* m_insert_after_action{nullptr};
  ActionMenu* m_insert_into_action{nullptr};
  QAction* m_remove_action{nullptr};

  //!< toolbar version without enable/disable features
  sup::gui::ProxyAction* m_remove_toolbar_action{nullptr};

  QAction* m_move_up_action{nullptr};
  QAction* m_move_down_action{nullptr};

  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_after_action{nullptr};
  QAction* m_paste_into_action{nullptr};

  QAction* m_undo_action{nullptr};
  QAction* m_redo_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
