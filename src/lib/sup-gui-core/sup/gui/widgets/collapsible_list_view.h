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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{

class CollapsibleToolBar;

/**
 * @brief The CollapsibleListView class represent a column of collapsible panels displayed one under
 * another.
 *
 * @details Each panel contains user widget and can be collapsed/expanded. When expanded, the place
 * occupied by the panel can be changed by draging a splitter. Each panel can contain a list of
 * actions associated with the widget. When panel is collapsed, actions are hidden.
 */
class CollapsibleListView : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleListView(QWidget* parent = nullptr);

  /**
   * @brief Adds widget to the splitter.
   *
   * @param Widget is added as it is, without collapsible bar and menus.
   */
  void AddWidget(QWidget* content);

  /**
   * @brief Adds widget as collapsible panel.
   *
   * @param content User widget to show inside collapsible panel.
   * @param actions Associated widget actions.
   *
   * @return A collapsible tool bar which was created for handle panel collapse.
   */
  CollapsibleToolBar* AddCollapsibleWidget(QWidget* content, const QList<QAction*>& actions);

  /**
   * @brief Returns internal splitter which is the basis of the whole thing.
   */
  QSplitter* GetSplitter() const;

private:
  QSplitter* m_splitter{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_LIST_VIEW_H_
