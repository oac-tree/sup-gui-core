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

#include "configuration_tab_widget.h"

#include <sup/gui/components/tree_helper.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_textpanel.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_treepanel.h>
#include <sup/gui/views/codeeditor/code_editor.h>
#include <sup/gui/views/codeeditor/code_view.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QMenu>
#include <QTabWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

ConfigurationTabWidget::ConfigurationTabWidget(ISettingsManager& settings_manager,
                                               QWidget* parent_widget)
    : SessionItemWidget(parent_widget)
    , m_tree_panel(new AnyValueEditorTreePanel(settings_manager))
    , m_text_panel(new AnyValueEditorTextPanel)
    , m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tab_widget);

  AddTab(m_tree_panel, "Configuration Tree",
         "Configuration editor with tree view of the configuration structure and properties");
  AddTab(m_text_panel, "JSON view",
         "Configuration view with JSON representation of the configuration");

  m_tab_widget->setTabPosition(QTabWidget::South);

  connect(m_tree_panel->GetTreeView(), &QTreeView::customContextMenuRequested, this,
          &ConfigurationTabWidget::OnContextMenuRequest);
}

void ConfigurationTabWidget::SetItem(mvvm::SessionItem* configuration_item)
{
  m_configuration_item = configuration_item;
  // configuration_item is a container with a single AnyValueItem
  m_tree_panel->SetAnyValueItemContainer(configuration_item);
  m_tree_panel->GetTreeView()->expandAll();
  m_text_panel->SetAnyValueItemContainer(configuration_item);
}

const mvvm::SessionItem* ConfigurationTabWidget::GetItem() const
{
  return m_configuration_item;
}

QTabWidget* ConfigurationTabWidget::GetTabWidget() const
{
  return m_tab_widget;
}

QAbstractScrollArea* ConfigurationTabWidget::GetScrollArea(ConfigurationViewMode mode) const
{
  if (mode == ConfigurationViewMode::kTree)
  {
    return m_tree_panel->GetTreeView();
  }

  if (mode == ConfigurationViewMode::kJSON)
  {
    return m_text_panel->GetCodeView()->GetCodeEditor();
  }

  return nullptr;
}

QTreeView* ConfigurationTabWidget::GetMainTreeView() const
{
  return m_tree_panel->GetTreeView();
}

void ConfigurationTabWidget::AddTab(QWidget* widget, const QString& label, const QString& tooltip)
{
  m_tab_widget->addTab(widget, label);
  m_tab_widget->setTabToolTip(m_tab_widget->indexOf(widget), tooltip);
}

void ConfigurationTabWidget::OnContextMenuRequest(const QPoint& point)
{
  QMenu menu;
  menu.setToolTipsVisible(true);

  SetupCollapseExpandMenu(point, menu, *m_tree_panel->GetTreeView());
  menu.exec(m_tree_panel->GetTreeView()->mapToGlobal(point));
}

}  // namespace sup::gui
