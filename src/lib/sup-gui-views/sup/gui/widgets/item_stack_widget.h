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

#ifndef SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_
#define SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_

#include <QList>
#include <QWidget>
#include <memory>
#include <sup/gui/widgets/settings_callbacks.h>

class QStackedWidget;
class QAction;
class QActionGroup;
class QToolBar;
class QMenu;

namespace sup::gui
{

class PanelToolBar;

/**
 * @brief The ItemStackWidget class is a stack of widgets with wide menubar and 3-dot menu at the
 * right corner.
 *
 * @details Every widget in a stack has own set of associated actions.
 */
class ItemStackWidget : public QWidget
{
  Q_OBJECT

public:

  explicit ItemStackWidget(QWidget* parent_widget = nullptr);

  explicit ItemStackWidget(const QString& settings_group_name, QWidget* parent = nullptr);

  ~ItemStackWidget() override;

  /**
   * @brief Adds a widget to the stack.
   */
  void AddWidget(QWidget* widget);

  /**
   * @brief Adds a widget to the stack.
   *
   * @param control_actions List of actions associated with the given widget to show on toolbar.
   * @param is_always_visible Flag which force all actions to be visible even if widget is hidden of
   * the current stack widget.
   */
  void AddWidget(QWidget* widget, const QList<QAction*>& control_actions,
                 bool is_always_visible = false);

  /**
   * @brief Returns current widget's index visible in a stack.
   */
  int GetCurrentIndex() const;

  /**
   * @brief Set widget with given index visible in a stack.
   */
  void SetCurrentIndex(int index);

  /**
   * @brief Returns widget which is currently visible.
   */
  QWidget* GetCurrentWidget() const;

  /**
   * @brief Sets given widget visible.
   */
  void SetCurrentWidget(QWidget* widget);

  /**
   * @brief Read settings from persistent storage using function provided.
   */
  void ReadSettings(const read_variant_func_t& read_func);

  /**
   * @brief Write settings to persistent storage using function provided.
   */
  void WriteSettings(const write_variant_func_t& write_func);

private:
  /**
   * @brief Adds entry to 3-dot corner menu to switch to this widget.
   */
  void AddMenuEntry(QWidget* widget);

  /**
   * @brief Add guest actions to the toolbar.
   *
   * @param actions The list of action to add.
   * @param is_always_visible The flag which forces actions to be visible for any currently active
   * widget.
   */
  void AddGuestActions(const QList<QAction*>& actions, bool is_always_visible);

  /**
   * @brief Updates visibility of guest actions.
   *
   * @details Actions of currenly visible widget will be visible too, and hidden for others (unless
   * is_always_visible flag is present);
   */
  void UpdateControlElementsVisibility();

  /**
   * @brief The GuestToolBarData struct contains handles (special actions) allowing to hide things
   * on a toolbar.
   */
  struct GuestToolBarData
  {
    QList<QAction*> actions;  //! actions to hide guest actions
    bool is_always_visible{false};
  };

  QStackedWidget* m_stacked_widget{nullptr};
  //!< Tech info to hide widget actions. The size of the list matches number of widgets in a stack.
  QList<GuestToolBarData> m_toolbar_data;
  std::unique_ptr<QMenu> m_widget_selection_menu;
  PanelToolBar* m_main_toolbar{nullptr};

  //!< action group for corner menu to show bullet list with currently selected widget marked
  QActionGroup* m_action_group{nullptr};

  QString m_settings_group_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_ITEM_STACK_WIDGET_H_
