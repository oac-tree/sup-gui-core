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

#ifndef SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_
#define SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_

#include <QToolBar>

class QLabel;
class QAction;
class QToolButton;
class QMenu;

namespace sup::gui
{

/**
 * @brief The PanelToolBar class represents a toolbar with a 3-dot menu selector.
 *
 * @details The toolbar has the following layout: [label, guest actions, spacer, 3dots-menu]. It's a
 * part of ItemStackWidget and its main purpose to switch between different widgets and show current
 * actions of the currently active widget.
 */
class PanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit PanelToolBar(QWidget* parent = nullptr);

  /**
   * @brief Sets toolbar label.
   */
  void SetText(const QString& text);

  /**
   * @brief Insert element after the label, or last insert element.
   *
   * Push button representing dots and spacer will always stay at the right.
   *
   * @param widget The widget to insert.
   * @return Associated action to controlor widget visibility.
   */
  QAction* InsertElement(QWidget* widget);

  /**
   * @brief Insert element after the label, or last insert element.
   *
   * Push button representing dots and spacer will always stay at the right.
   *
   * @param widget The widget to insert.
   * @return Associated action to controlor widget visibility.
   */
  QAction* InsertElement(QAction* action);

  /**
   * @brief Assign given menu to 3-dot button on the right.
   *
   * The ownership for menu will remain on the user side.
   */
  void SetDotsMenu(QMenu* dots_menu);

  /**
   * @brief Appends separator to the last guest element, but before 3-dots menu.
   *
   * @return Associated action to hide separator.
   */
  QAction* AppendSeparator();

private:
  void InsertStrech();
  void SetupDotsButton();

  QLabel* m_label{nullptr};
  QAction* m_label_action{nullptr};
  QAction* m_spacer_action{nullptr};
  QToolButton* m_dots_button{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_PANEL_TOOLBAR_H_
