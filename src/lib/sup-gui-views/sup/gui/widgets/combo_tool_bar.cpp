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

#include "combo_tool_bar.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/style/style_helper.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QAction>
#include <QMenu>
#include <QPainter>
#include <QToolButton>

namespace sup::gui
{

ComboToolBar::ComboToolBar(const NamesCallback& entry_callback, QWidget* parent_widget)
    : ComboToolBar(ComparePanelType::kDefault, entry_callback, parent_widget)
{
}

ComboToolBar::ComboToolBar(ComparePanelType panel_type, const NamesCallback& entry_callback,
                           QWidget* parent_widget)
    : QToolBar(parent_widget)
    , m_panel_type(panel_type)
    , m_available_items(entry_callback)
    , m_select_item_menu(CreateSelectItemMenu())
    , m_selector_button(new QToolButton(this))
    , m_reference_button(new QToolButton(this))
    , m_split_horizontally_button(new QToolButton(this))
    , m_close_current_view_button(new QToolButton(this))
{
  if (!m_available_items)
  {
    throw RuntimeException("Item callback is not set");
  }

  setIconSize(utils::NarrowToolBarIconSize());

  m_selector_button->setMenu(m_select_item_menu.get());
  m_selector_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_selector_button->setPopupMode(QToolButton::MenuButtonPopup);
  m_selector_button->setToolTip("Select one of the existing items for editing in this widget");
  m_selector_button->setAutoRaise(true);
  m_selector_button->setFocusPolicy(Qt::NoFocus);
  m_selector_button->setMaximumWidth(mvvm::style::UnitSize(25));
  addWidget(m_selector_button);

  InsertStretch();

  m_reference_button->setIcon(utils::FindIcon("radiobox-blank"));
  m_reference_button->setToolTip("Make this a reference panel");
  auto action = addWidget(m_reference_button);
  action->setVisible(m_panel_type == ComparePanelType::kReference);

  auto on_action = [this]()
  {
    m_panel_state.Toggle(ComparePanelState::kReference);
    UpdateAppearance();
    m_reference_button->setIcon(m_panel_state.IsSet(ComparePanelState::kReference)
                                    ? utils::FindIcon("radiobox-marked")
                                    : utils::FindIcon("radiobox-blank"));
    emit referenceRequest(m_panel_state.IsSet(ComparePanelState::kReference));
  };
  connect(m_reference_button, &QToolButton::clicked, this, on_action);

  m_split_horizontally_button->setToolTip("Split current view to the right");
  m_split_horizontally_button->setIcon(utils::FindIcon("view-split-left-right-16"));
  m_split_horizontally_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  connect(m_split_horizontally_button, &QToolButton::clicked, this,
          &ComboToolBar::splitViewRequest);
  addWidget(m_split_horizontally_button);

  m_close_current_view_button->setToolTip("Close this view");
  m_close_current_view_button->setIcon(utils::FindIcon("dialog-close-16"));
  m_close_current_view_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  connect(m_close_current_view_button, &QToolButton::clicked, this,
          &ComboToolBar::closeViewRequest);
  addWidget(m_close_current_view_button);
}

ComboToolBar::~ComboToolBar() = default;

void ComboToolBar::SetPanelState(mvvm::Flags<ComparePanelState> state)
{
  m_panel_state = state;
  UpdateAppearance();
}

void ComboToolBar::SetPanelFlag(ComparePanelState flag, bool value)
{
  m_panel_state.Set(flag, value);
  UpdateAppearance();
}

mvvm::Flags<ComparePanelState> ComboToolBar::GetPanelState() const
{
  return m_panel_state;
}

void ComboToolBar::SetCurrentIndex(std::int32_t index)
{
  auto items = m_available_items();

  // invalid index is ignored
  if (index < 0 || index >= static_cast<std::int32_t>(items.size()))
  {
    return;
  }

  if (index == m_current_index)
  {
    return;
  }

  m_current_index = index;
  m_selector_button->setText(m_available_items().at(m_current_index));
  emit selectItemRequest(index);
}

int32_t ComboToolBar::GetCurrentIndex() const
{
  return m_current_index;
}

QString ComboToolBar::GetCurrentText() const
{
  return m_selector_button->text();
}

QMenu* ComboToolBar::GetMenu()
{
  return m_select_item_menu.get();
}

void ComboToolBar::UpdateAppearance()
{
  // need to update button text in case the item was renamed
  if (m_current_index >= 0)
  {
    // in theory UpdateAppearance() can be called programmatically at any moment, but we assume that
    // menu is not open at the moment of calling this method, so we don't care about its state
    m_selector_button->setText(m_available_items().at(m_current_index));
  }

  // we don't want to allow closing the last editor in the splitter
  m_close_current_view_button->setDisabled(m_panel_state.IsSet(ComparePanelState::kLastPanel));

  // update reference icon
  m_reference_button->setIcon(m_panel_state.IsSet(ComparePanelState::kReference)
                                  ? utils::FindIcon("radiobox-marked")
                                  : utils::FindIcon("radiobox-blank"));

  update();  // to repaint green line
}

void ComboToolBar::paintEvent(QPaintEvent* event)
{
  static const QColor kMediumGreen(39, 189, 114);  // #27bd72

  // green line is painted if panel is active and it is not the last panel in the splitter
  if (m_panel_state.IsSet(ComparePanelState::kActivePanel)
      && !m_panel_state.IsSet(ComparePanelState::kLastPanel))
  {
    // thin horizontal green line at the bottom
    QPainter painter(this);
    QPen pen(kMediumGreen);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(0, height() - 1, width(), height() - 1);
  }

  QToolBar::paintEvent(event);
}

void ComboToolBar::InsertStretch()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> ComboToolBar::CreateSelectItemMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &ComboToolBar::OnAboutToShowMenu);
  return result;
}

void ComboToolBar::OnAboutToShowMenu()
{
  if (!m_available_items)
  {
    return;
  }

  auto menu = m_select_item_menu.get();
  menu->clear();

  std::int32_t index{0};
  for (const auto& item_name : m_available_items())
  {
    auto action = menu->addAction(item_name);
    auto on_action = [this, index]() { SetCurrentIndex(index); };
    connect(action, &QAction::triggered, this, on_action);
    ++index;
  }
}

}  // namespace sup::gui
