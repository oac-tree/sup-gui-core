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

#ifndef SUP_GUI_WIDGETS_COMBO_TOOL_BAR_H_
#define SUP_GUI_WIDGETS_COMBO_TOOL_BAR_H_

#include <QToolBar>
#include <functional>

class QMenu;
class QToolButton;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

/**
 * @brief The ComboToolBar class contains a combo selector to switch between different documents,
 * and control elements to split/close views.
 */
class ComboToolBar : public QToolBar
{
  Q_OBJECT

public:
  using ItemCallback = std::function<std::vector<mvvm::SessionItem*>()>;

  explicit ComboToolBar(const ItemCallback& item_callback, QWidget* parent_widget = nullptr);
  ~ComboToolBar() override;

  ComboToolBar(const ComboToolBar&) = delete;
  ComboToolBar& operator=(const ComboToolBar&) = delete;
  ComboToolBar(ComboToolBar&&) noexcept = delete;
  ComboToolBar& operator=(ComboToolBar&&) noexcept = delete;

  /**
   * @brief Shows the toolbar as active/inactive.
   *
   * Active toolbar corresponds to the widget where the user clicked last, and it has a thin green
   * line at the bottom.
   */
  void ShowAsActive(bool value);

  /**
   * @brief Marks this toolbar as the last editor in the splitter.
   */
  void ShowAsLastEditor(bool value);

  /**
   * @brief Updates the item selection menu to highlight given item.
   */
  void UpdateSelectionMenu(mvvm::SessionItem* selected_item);

  /**
   * @brief Returns item name selected in a toolbar combo.
   */
  QString GetSelectedItemName() const;

signals:
  void splitViewRequest();
  void closeViewRequest();
  void selectItemRequest(const mvvm::SessionItem* item);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void InsertStretch();
  std::unique_ptr<QMenu> CreateSelectItemMenu();
  void OnAboutToShowMenu();

  ItemCallback m_available_items;
  std::unique_ptr<QMenu> m_select_item_menu;
  QToolButton* m_selector_button{nullptr};
  QToolButton* m_split_horizontally_button{nullptr};
  QToolButton* m_close_current_view_button{nullptr};
  bool m_show_as_active{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COMBO_TOOL_BAR_H_
