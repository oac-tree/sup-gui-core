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

#include <QObject>
#include <memory>

class QAction;
class QMenu;

namespace sup::gui
{

class ActionMenu;

/**
 * @brief The AnyValueEditorActions class contains collection of actions to edit AnyValue tree.
 */
class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorActions(QObject* parent = nullptr);
  ~AnyValueEditorActions() override;

  QList<QAction*> GetActions() const;

signals:
  void AddEmptyAnyValueRequest();
  void AddAnyValueStructRequest();
  void AddAnyValueArrayRequest();
  void AddAnyValueScalarRequest(const QString& type_name);
  void ImportFromFileRequest();
  void RemoveSelectedRequest();
  void MoveUpRequest();
  void MoveDownRequest();

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateAddAnyValueMenu();

  std::unique_ptr<QMenu> m_create_anyvalue_menu;

  ActionMenu* m_add_anyvalue_action{nullptr};
  QAction* m_remove_selected_action{nullptr};
  QAction* m_move_up_action{nullptr};
  QAction* m_move_down_action{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
