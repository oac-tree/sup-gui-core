/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_treepanel.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/viewmodel/anyvalue_viewmodel.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/tree_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("AnyValueEditor");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";

}  // namespace

namespace sup::gui
{

AnyValueEditorTreePanel::AnyValueEditorTreePanel(mvvm::ApplicationModel *model, QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<sup::gui::AnyValueViewModel>(m_tree_view))
{
  setWindowTitle("AnyValue tree");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tree_view);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setEditTriggers(QAbstractItemView::SelectedClicked
                               | QAbstractItemView::EditKeyPressed
                               | QAbstractItemView::DoubleClicked);
  m_tree_view->setAlternatingRowColors(true);
  m_custom_header->setStretchLastSection(true);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          sup::gui::CreateOnCustomMenuCallback(*m_tree_view));

  m_component_provider->SetApplicationModel(model);
  m_tree_view->expandAll();

  ReadSettings();

  AdjustTreeAppearance();
}

AnyValueEditorTreePanel::~AnyValueEditorTreePanel()
{
  WriteSettings();
}

AnyValueItem *AnyValueEditorTreePanel::GetSelectedItem() const
{
  return m_component_provider->GetSelected<sup::gui::AnyValueItem>();
}

void AnyValueEditorTreePanel::SetSelected(mvvm::SessionItem *item)
{
  m_component_provider->SetSelectedItem(item);
  auto index_of_inserted = m_component_provider->GetViewModel()->GetIndexOfSessionItem(item);
  if (!index_of_inserted.empty())
  {
    m_tree_view->setExpanded(index_of_inserted.front(), true);
  }
}

QTreeView *AnyValueEditorTreePanel::GetTreeView()
{
  return m_tree_view;
}

void AnyValueEditorTreePanel::ReadSettings()
{
  const QSettings settings;
  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void AnyValueEditorTreePanel::WriteSettings()
{
  QSettings settings;
  settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
}

void AnyValueEditorTreePanel::AdjustTreeAppearance()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
}

}  // namespace sup::gui
