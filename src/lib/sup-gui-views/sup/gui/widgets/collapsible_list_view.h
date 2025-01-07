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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{

class CollapsibleToolBar;

/**
 * @brief The CollapsibleListView class represents a column of collapsible panels.
 *
 * Each panel contains a tiny toolbar on top of the user widget and can be collapsed/expanded. When
 * expanded, the place occupied by the panel can be changed by dragging a splitter. Each panel's
 * toolbar contains a list of actions associated with the widget. When the panel is collapsed,
 * actions are hidden.
 */
class CollapsibleListView : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleListView(QWidget* parent_widget = nullptr);

  /**
   * @brief Adds user widget to the splitter.
   *
   * Widget is added as it is, without collapsible bar and menus.
   *
   * @param content The widget to add.
   */
  void AddWidget(QWidget* content);

  /**
   * @brief Adds widget as a collapsible panel.
   *
   * @param content User widget to show inside collapsible panel.
   * @param actions Associated widget actions.
   * @param set_initially_expanded Show widget expanded according at first show.
   *
   * @return A collapsible tool bar which was created for handle panel collapse.
   */
  CollapsibleToolBar* AddCollapsibleWidget(QWidget* content, const QList<QAction*>& actions,
                                           bool set_initially_expanded = true);

  /**
   * @brief Returns internal splitter which is the basis of the whole thing.
   */
  QSplitter* GetSplitter() const;

private:
  QSplitter* m_splitter{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
