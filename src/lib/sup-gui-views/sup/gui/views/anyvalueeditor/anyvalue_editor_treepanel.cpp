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

#include "anyvalue_editor_treepanel.h"

#include <sup/gui/components/anyvalue_editor_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/views/anyvalueeditor/tree_view_component_provider.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/model/item_selection.h>

#include <QLineEdit>
#include <QSettings>
#include <QTimer>
#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

const QString kHeaderStateSettingName("AnyValueEditor/header_state");
const std::vector<int> kDefaultColumnStretch({2, 1, 1});

/**
 * @brief Time to wait after the last keystroke before the filter pattern is applied.
 *
 * Used to spead-up large tree re-filtering, to ignore filter updates while user is still typing.
 */
constexpr int kFilterDelayMsec{300};

}  // namespace

AnyValueEditorTreePanel::AnyValueEditorTreePanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_line_edit(new QLineEdit)
    , m_custom_header(new CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_filter_timer(new QTimer(this))
    , m_component_provider(std::make_unique<TreeViewComponentProvider>(nullptr, m_tree_view))
{
  setWindowTitle("AnyValue tree");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_line_edit->setClearButtonEnabled(true);
  m_line_edit->setPlaceholderText("Filter pattern");

  layout->addWidget(m_tree_view);
  layout->addWidget(m_line_edit);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setEditTriggers(QAbstractItemView::EditKeyPressed
                               | QAbstractItemView::DoubleClicked);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  m_tree_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

  m_filter_timer->setSingleShot(true);
  m_filter_timer->setInterval(kFilterDelayMsec);

  auto on_timeout = [this]() { m_component_provider->SetFilterPattern(m_line_edit->text()); };
  connect(m_filter_timer, &QTimer::timeout, this, on_timeout);

  // restarting the timer on every keystroke, so that only the final pattern reaches the proxy model
  auto on_text = [this]() { m_filter_timer->start(); };
  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);

  connect(m_component_provider.get(), &TreeViewComponentProvider::SelectionChanged, this,
          &AnyValueEditorTreePanel::SelectionChanged);
}

AnyValueEditorTreePanel::~AnyValueEditorTreePanel() = default;

void AnyValueEditorTreePanel::SetAnyValueItemContainer(mvvm::SessionItem* container)
{
  m_component_provider->SetItem(container);
  AdjustTreeAppearance();
}

std::vector<const AnyValueItem *> AnyValueEditorTreePanel::GetSelectedItems() const
{
  auto selection = m_component_provider->GetSelection();
  return selection.GetItems<AnyValueItem>();
}

void AnyValueEditorTreePanel::SetSelected(const mvvm::SessionItem* item)
{
  m_component_provider->SetSelection(mvvm::ItemSelection(item));
  auto indices_of_inserted = m_component_provider->GetViewIndexes(item);
  if (!indices_of_inserted.empty())
  {
    m_tree_view->setExpanded(indices_of_inserted.front(), true);
    if (EnableInstantFieldNameEdit(*item))
    {
      if (auto editor = m_tree_view->indexWidget(m_last_edited_index); editor != nullptr)
      {
        m_tree_view->itemDelegate()->closeEditor(editor, QAbstractItemDelegate::NoHint);
      }
      m_last_edited_index = indices_of_inserted.front();
      m_tree_view->edit(indices_of_inserted.front());
    }
  }
}

QTreeView* AnyValueEditorTreePanel::GetTreeView()
{
  return m_tree_view;
}

void AnyValueEditorTreePanel::AdjustTreeAppearance()
{
  m_custom_header->AdjustColumnsWidth();
}

}  // namespace sup::gui
