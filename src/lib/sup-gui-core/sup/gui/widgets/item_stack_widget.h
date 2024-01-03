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

#ifndef SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_
#define SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_

#include <QList>
#include <QWidget>
#include <memory>

class QStackedWidget;
class QAction;
class QActionGroup;
class QToolBar;
class QMenu;

namespace sup::gui
{

class PanelToolBar;

/**
 * @brief The ItemStackWidget class is a stack of widgets with wide menubar and dots-menu at the
 * right corner.
 */

class ItemStackWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ItemStackWidget(QWidget* parent = nullptr);
  ~ItemStackWidget() override;

  void AddWidget(QWidget* widget);

  void AddWidget(QWidget* widget, const QList<QAction*>& control_actions,
                 bool is_always_visible = false);

  int GetCurrentIndex() const;

  void SetCurrentIndex(int index);

  QWidget* GetCurrentWidget() const;

  void SetCurrentWidget(QWidget* widget);

private:
  void AddMenuEntry(QWidget* widget);
  void AddGuestActions(const QList<QAction*>& actions, bool is_always_visible);
  void UpdateControlElementsVisibility();

  struct GuestToolBarData
  {
    QList<QAction*> actions;  //! actions to hide guest toolbar
    bool is_always_visible{false};
  };

  QStackedWidget* m_stacked_widget{nullptr};
  QList<GuestToolBarData> m_toolbar_data;
  std::unique_ptr<QMenu> m_widget_selection_menu;
  PanelToolBar* m_main_toolbar{nullptr};
  QActionGroup* m_action_group{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_
