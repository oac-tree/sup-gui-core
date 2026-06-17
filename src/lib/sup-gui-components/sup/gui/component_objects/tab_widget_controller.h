/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_COMPONENT_OBJECTS_TAB_WIDGET_CONTROLLER_H_
#define SUP_GUI_COMPONENT_OBJECTS_TAB_WIDGET_CONTROLLER_H_

#include <QObject>
#include <vector>

class QTabWidget;

namespace sup::gui
{

/**
 * @brief The TabWidgetController class synchronises the current tab of multiple QTabWidget
 * instances.
 *
 * After registering two or more tab widgets via AddTabWidget(), switching the current tab in one
 * widget is immediately mirrored to all other registered widgets. Synchronisation can be
 * temporarily suspended with SetActive(false).
 */
class TabWidgetController : public QObject
{
  Q_OBJECT

public:
  explicit TabWidgetController(QObject* parent_object = nullptr);
  ~TabWidgetController() override;

  TabWidgetController(const TabWidgetController&) = delete;
  TabWidgetController& operator=(const TabWidgetController&) = delete;
  TabWidgetController(TabWidgetController&&) = delete;
  TabWidgetController& operator=(TabWidgetController&&) = delete;

  /**
   * @brief Adds a tab widget to the synchronisation group.
   *
   * Does nothing if @p tab_widget is nullptr or already registered.
   */
  void AddTabWidget(QTabWidget* tab_widget);

  /**
   * @brief Removes a tab widget from the synchronisation group.
   *
   * Does nothing if @p tab_widget is nullptr or not registered.
   */
  void RemoveTabWidget(QTabWidget* tab_widget);

  /**
   * @brief Enables or disables tab synchronisation without unregistering widgets.
   *
   * When synchronisation is switched from disabled to enabled, all registered tab widgets are
   * realigned to the current tab of the first widget in the list.
   */
  void SetActive(bool value);

  /**
   * @brief Returns true when synchronisation is active.
   */
  bool IsActive() const;

private:
  void OnCurrentChanged(QTabWidget* source, int index);

  /**
   * @brief Aligns all registered tab widgets to the current tab of the first widget in the list.
   */
  void SyncToFirstWidget();

  std::vector<QTabWidget*> m_tab_widgets;
  bool m_is_active{true};
  bool m_is_propagating{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENT_OBJECTS_TAB_WIDGET_CONTROLLER_H_
