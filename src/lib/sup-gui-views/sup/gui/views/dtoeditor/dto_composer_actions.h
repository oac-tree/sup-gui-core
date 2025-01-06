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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_ACTIONS_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_ACTIONS_H_

#include <QObject>

class QAction;

namespace sup::gui
{

/**
 * @brief The DtoComposerActions class contains a collection of the main actions of DtoComposerView.
 *
 * Actions are related to multiple AnyValueItem editing and can populate the main application
 * menubar, tabs context menu, etc. The class only creates actions and provides signaling when they
 * are triggered. The logic is handled by DtoComposerActionHandler.
 */
class DtoComposerActions : public QObject
{
  Q_OBJECT

public:
  explicit DtoComposerActions(QObject* parent = nullptr);

  QList<QAction*> GetActions() const;

signals:
  void AddNewTabRequest();
  void DuplicateCurrentTabRequest();
  void RemoveCurrentTabRequest();

private:
  QAction* m_add_new_tab_action{nullptr};
  QAction* m_add_duplicate_tab_action{nullptr};
  QAction* m_remove_current_tab_action{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_ACTIONS_H_
