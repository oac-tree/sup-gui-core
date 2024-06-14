/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/gui/viewmodel/tree_view_component_provider.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <QLineEdit>
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

AnyValueEditorTreePanel::AnyValueEditorTreePanel(mvvm::SessionModelInterface *model,
                                                 QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_line_edit(new QLineEdit)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(std::make_unique<TreeViewComponentProvider>(model, m_tree_view))
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
  m_custom_header->setStretchLastSection(true);
  m_tree_view->expandAll();

  ReadSettings();

  AdjustTreeAppearance();

  auto on_text = [this]() { m_component_provider->SetFilterPattern(m_line_edit->text()); };
  connect(m_line_edit, &QLineEdit::textChanged, this, on_text);

  connect(m_component_provider.get(), &TreeViewComponentProvider::SelectedItemChanged, this,
          &AnyValueEditorTreePanel::SelectedItemChanged);
}

AnyValueEditorTreePanel::~AnyValueEditorTreePanel()
{
  WriteSettings();
}

void AnyValueEditorTreePanel::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  m_component_provider->SetItem(container);
}

AnyValueItem *AnyValueEditorTreePanel::GetSelectedItem() const
{
  auto item = const_cast<mvvm::SessionItem *>(m_component_provider->GetSelectedItem());
  return dynamic_cast<AnyValueItem *>(item);
}

void AnyValueEditorTreePanel::SetSelected(mvvm::SessionItem *item)
{
  m_component_provider->SetSelectedItem(item);
  auto indices_of_inserted = m_component_provider->GetViewIndices(item);
  if (!indices_of_inserted.empty())
  {
    m_tree_view->setExpanded(indices_of_inserted.front(), true);
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
