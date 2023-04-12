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

#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/model/application_model.h>

#include <sup/gui/viewmodel/anyvalue_viewmodel.h>
#include <sup/gui/model/anyvalue_item.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

AnyValueEditorTreePanel::AnyValueEditorTreePanel(mvvm::ApplicationModel *model, QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<sup::gui::AnyValueViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tree_view);

  m_component_provider->SetApplicationModel(model);
  m_tree_view->expandAll();
}

AnyValueEditorTreePanel::~AnyValueEditorTreePanel() = default;

AnyValueItem *AnyValueEditorTreePanel::GetSelectedItem() const
{
  return m_component_provider->GetSelected<sup::gui::AnyValueItem>();
}

}  // namespace sup::gui
