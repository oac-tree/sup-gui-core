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

#include "anyvalue_list_widget.h"

#include <sup/gui/model/configuration_container_item.h>

#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

AnyValueListWidget::AnyValueListWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_tree_view(new QTreeView), m_component_provider(CreateProvider())

{
  setWindowTitle("CONFIGURATIONS");
  setToolTip("List of currently opened configurations");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tree_view);
}

void AnyValueListWidget::SetConfigurationContainer(ConfigurationContainerItem* container)
{
  m_component_provider->SetItem(container);
}

std::unique_ptr<mvvm::ItemViewComponentProvider> AnyValueListWidget::CreateProvider() const
{
  auto viewmodel = std::make_unique<mvvm::TopItemsViewModel>(nullptr);

  return std::make_unique<mvvm::ItemViewComponentProvider>(std::move(viewmodel), m_tree_view);
}

}  // namespace sup::gui
