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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_TOOLBAR_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_TOOLBAR_H_

#include <QFrame>
#include <QList>

class QToolBar;
class QLabel;
class QToolButton;
class QAction;

namespace sup::gui
{

/**
 * @brief The CollapsibleToolBar class is a narrow toolbar with the possibility to collapse/expand
 * controlled widget.
 *
 * @details Toolbar buttons are shown only when expanded. Looks like this:
 * > LABEL <STRETCH> BUTTON BUTTON BUTTON
 * This toolbar is intended to function in a splitter as a part of CollapsibleListView.
 */
class CollapsibleToolBar : public QFrame
{
  Q_OBJECT

public:
  explicit CollapsibleToolBar(QWidget* parent_widget = nullptr);

  /**
   * @brief Sets text next to collapse/expand icons.
   *
   * @param text Label text.
   * @param text_tooltip Label tooltip.
   */
  void SetText(const QString& text, const QString& text_tooltip = {});

  /**
   * @brief Sets the widget which will be collapsed/expanded.
   */
  void SetControlledWidget(QWidget* widget);

  /**
   * @brief Adds widget to a toolbar.
   */
  void AddWidget(QWidget* widget);

  /**
   * @brief Appends list of actions to a toolbar.
   */
  void AddActions(const QList<QAction*>& actions);

  /**
   * @brief Returns underlying toolbar.
   */
  QToolBar* GetToolBar();

  /**
   * @brief Set expanded state according to the given value.
   */
  void SetExpanded(bool value);

protected:
  /**
   * @brief Handles icon and expand status if controlled widget has been change programmatically
   */
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  /**
   * @brief Updates toolbar appearance depending on collapse/expand status.
   */
  void UpdateToolBar();

  /**
   * @brief Updates collapse/expand icon.
   */
  void UpdateIcon();

  void InsertStrech();

  QToolBar* m_tool_bar{nullptr};
  QToolButton* m_expand_button{nullptr};
  QLabel* m_label{nullptr};
  bool m_is_expanded{true};
  QWidget* m_controlled_widget{nullptr};
  QList<QAction*> m_toolbar_actions;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_TOOLBAR_H_
