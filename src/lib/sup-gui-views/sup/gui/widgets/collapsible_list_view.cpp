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

#include "collapsible_list_view.h"

#include "collapsible_toolbar.h"
#include "collapsible_widget.h"
#include "custom_splitter.h"
#include "settings_splitter.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
const QString kDefaultSettingGroup = "CollapsibleListView";
}

CollapsibleListView::CollapsibleListView(QWidget* parent_widget)
    : CollapsibleListView(kDefaultSettingGroup, parent_widget)
{
}

CollapsibleListView::CollapsibleListView(const QString& setting_name, QWidget* parent_widget)
    : QWidget(parent_widget), m_custom_splitter(new CustomSplitter(setting_name))
{
  InitSplitter(m_custom_splitter);
}

CollapsibleListView::CollapsibleListView(std::string group, ISettingsManager& settings_manager,
                                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_settings_splitter(new SettingsSplitter(std::move(group), settings_manager))
{
  InitSplitter(m_settings_splitter);
}

void CollapsibleListView::AddWidget(QWidget* content)
{
  m_splitter->addWidget(content);
}

CollapsibleToolBar* CollapsibleListView::AddCollapsibleWidget(QWidget* content,
                                                              const QList<QAction*>& actions,
                                                              bool set_initially_expanded)
{
  if (content == nullptr)
  {
    throw RuntimeException("CollapsibleListView: can't add a null widget");
  }
  auto collapsible_widget = new CollapsibleWidget(content, actions);

  collapsible_widget->AddToSplitter(m_splitter);
  collapsible_widget->SetExpanded(set_initially_expanded);

  return collapsible_widget->GetToolBar();
}

QSplitter* CollapsibleListView::GetSplitter() const
{
  return m_splitter;
}

void CollapsibleListView::ReadSettings()
{
  if (m_settings_splitter != nullptr)
  {
    m_settings_splitter->RestoreState();
    return;
  }
  m_custom_splitter->ReadSettings();
}

void CollapsibleListView::WriteSettings()
{
  if (m_custom_splitter != nullptr)
  {
    m_custom_splitter->WriteSettings();
  }
}

void CollapsibleListView::InitSplitter(QSplitter* splitter)
{
  m_splitter = splitter;
  m_splitter->setOrientation(Qt::Vertical);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_splitter);
}

}  // namespace sup::gui
