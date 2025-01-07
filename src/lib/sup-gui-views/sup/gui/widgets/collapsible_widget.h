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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{

class CollapsibleToolBar;

/**
 * @brief The CollapsibleWidget class is an envelope that contains a user widget with a narrow
 * toolbar on top.
 *
 * It is intended to be inserted in a splitter. The toolbar contains a collapse/expand button, which
 * will hide and show the user widget.
 */
class CollapsibleWidget : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param content The user widget which will be shown in collapsible panel.
   * @param parent_widget The parent widfget.
   */
  explicit CollapsibleWidget(QWidget* content, const QList<QAction*>& actions,
                             QWidget* parent_widget = nullptr);
  ~CollapsibleWidget() override;

  /**
   * @brief Add this collapsible widget to a given splitter.
   *
   * @param The splitter
   */
  void AddToSplitter(QSplitter* splitter);

  /**
   * @brief Make user widget expanded or collapsed.
   */
  void SetExpanded(bool value);

  /**
   * @brief Returns underlying toolbar.
   */
  CollapsibleToolBar* GetToolBar();

private:
  //!< a separate tool bar which doesn't belong to this widget
  CollapsibleToolBar* m_tool_bar{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_
