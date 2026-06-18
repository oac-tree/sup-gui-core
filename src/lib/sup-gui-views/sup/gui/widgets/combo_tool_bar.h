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

#include <sup/gui/components/component_types.h>

#include <mvvm/core/flags.h>

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
   * @param panel_type The type of the panel, which defines its appearance and available controls.
   * @param names_callback A callback allowing to retrieve list of strings which will sevre as menu
   * entries.
   * @param parent_widget The parent widget.
   */
  ComboToolBar(ComparePanelType panel_type, const NamesCallback& names_callback,
               QWidget* parent_widget = nullptr);

  explicit ComboToolBar(const NamesCallback& names_callback,
               QWidget* parent_widget = nullptr);


  ~ComboToolBar() override;

  ComboToolBar(const ComboToolBar&) = delete;
  ComboToolBar& operator=(const ComboToolBar&) = delete;
  ComboToolBar(ComboToolBar&&) noexcept = delete;
  ComboToolBar& operator=(ComboToolBar&&) noexcept = delete;

  /**
   * @brief Sets whole state of the panel.
   */
  void SetPanelState(mvvm::Flags<ComparePanelState> state);

  /**
   * @brief Sets individual state of the panel.
   */
  void SetPanelFlag(ComparePanelState flag, bool value);

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
   * Also when appearance flags are changed.
   */
  void UpdateAppearance();

signals:
  void splitViewRequest();
  void closeViewRequest();
  void referenceRequest(bool make_me_reference);
  void selectItemRequest(std::int32_t index);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void InsertStretch();
  std::unique_ptr<QMenu> CreateSelectItemMenu();
  void OnAboutToShowMenu();

  ComparePanelType m_panel_type{ComparePanelType::kDefault};
  NamesCallback m_available_items;
  std::unique_ptr<QMenu> m_select_item_menu;
  QToolButton* m_selector_button{nullptr};
  QToolButton* m_reference_button{nullptr};
  QToolButton* m_split_horizontally_button{nullptr};
  QToolButton* m_close_current_view_button{nullptr};
  std::int32_t m_current_index{-1};
  mvvm::Flags<ComparePanelState> m_panel_state;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COMBO_TOOL_BAR_H_
