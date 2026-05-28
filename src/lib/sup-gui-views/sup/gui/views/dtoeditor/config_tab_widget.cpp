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

#include "config_tab_widget.h"

#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_textpanel.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_treepanel.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

ConfigTabWidget::ConfigTabWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new QToolBar)
    , m_tree_panel(new AnyValueEditorTreePanel)
    , m_text_panel(new AnyValueEditorTextPanel)
    , m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setIconSize(utils::NarrowToolBarIconSize());
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  // to have fixed height when no actions are added
  m_tool_bar->setFixedHeight(utils::NarrowToolBarIconSize().height() + mvvm::style::UnitSize(1.9));

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tab_widget);

  AddTab(m_tree_panel, "Configuration Tree",
         "Configuration editor with tree view of the configuration structure and properties");
  AddTab(m_text_panel, "JSON view",
         "Configuration view with JSON representation of the configuration");

  m_tab_widget->setTabPosition(QTabWidget::South);

  connect(m_tab_widget, &QTabWidget::currentChanged, this, [this](int) { UpdateToolbarButtons(); });
  UpdateToolbarButtons();
}

void ConfigTabWidget::SetConfiguration(ConfigurationItem* configuration_item)
{
  (void)configuration_item;
}

void ConfigTabWidget::UpdateToolbarButtons()
{
  m_tool_bar->clear();
  m_tool_bar->addActions(m_tab_widget->currentWidget()->actions());
}

void ConfigTabWidget::AddTab(QWidget* widget, const QString& label, const QString& tooltip)
{
  m_tab_widget->addTab(widget, label);
  m_tab_widget->setTabToolTip(m_tab_widget->indexOf(widget), tooltip);
}

}  // namespace sup::gui
