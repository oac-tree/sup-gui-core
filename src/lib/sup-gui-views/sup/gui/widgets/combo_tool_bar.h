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

#include <QList>
#include <QString>
#include <QToolBar>
#include <functional>

class QMenu;
class QToolButton;

namespace sup::gui
{

/**
 * @brief The ComboToolBar class contains a combo selector to switch between different documents,
 * and control elements to split/close views.
 *
 * It uses callback to retrieve names and populate combo box with possible choice.
 *
 * Combo selector is implemented as a QToolButton with text. This provides better "flat" appearance
 * in the toolbar, than usual QComboBox.
 */
class ComboToolBar : public QToolBar
{
  Q_OBJECT

public:
  using NamesCallback = std::function<QList<QString>()>;

  /**
   * @brief Main c-tor.
   *
   * @param names_callback A callback allowing to retrieve list of strings which will sevre as menu
   * entries.
   * @param parent_widget The parent widget.
   */
  explicit ComboToolBar(const NamesCallback& names_callback, QWidget* parent_widget = nullptr);
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
   *
   * Disables "close" button, since the last widget can't be closed.
   */
  void ShowAsLastEditor(bool value);

  /**
   * @brief Updates the item selection menu to highlight entry with the given index.
   */
  void SetCurrentIndex(std::int32_t index);

  /**
   * @brief Returns index of the currently selected item in a toolbar combo.
   */
  std::int32_t GetCurrentIndex() const;

  /**
   * @brief Returns item name selected in a toolbar combo.
   */
  QString GetCurrentText() const;

  /**
   * @brief Returns underlying menu embedded in selector button.
   */
  QMenu* GetMenu();

  /**
   * @brief Updates the toolbar appearance.
   *
   * Should be called when the list of items is changed, to update the item selection menu.
   */
  void UpdateAppearance();

signals:
  void splitViewRequest();
  void closeViewRequest();
  void selectItemRequest(std::int32_t index);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void InsertStretch();
  std::unique_ptr<QMenu> CreateSelectItemMenu();
  void OnAboutToShowMenu();

  NamesCallback m_available_items;
  std::unique_ptr<QMenu> m_select_item_menu;
  QToolButton* m_selector_button{nullptr};
  QToolButton* m_split_horizontally_button{nullptr};
  QToolButton* m_close_current_view_button{nullptr};
  bool m_show_as_active{false};
  std::int32_t m_current_index{-1};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COMBO_TOOL_BAR_H_
