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

#ifndef SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_
#define SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_

#include <QToolBar>

class QToolBar;
class QLabel;
class QAction;
class QToolButton;
class QMenu;

namespace sup::gui
{

/**
 * @brief The PanelToolBar class repesents a toolbar with menu selector.
 */

class PanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit PanelToolBar(QWidget* parent = nullptr);

  void setText(const QString& text);

  void AddDotsMenu();

  QAction* InsertElement(QWidget* widget);
  QAction* InsertElement(QAction* action);

  void SetDotsMenu(QMenu* dots_menu);

  QAction* AppendSeparator();

private:
  void InsertStrech();

  QLabel* m_label{nullptr};
  QAction* m_spacer_action{nullptr};
  QToolButton* m_dots_button{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_
