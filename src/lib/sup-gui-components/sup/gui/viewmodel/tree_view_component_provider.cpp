/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "tree_view_component_provider.h"

#include <sup/gui/viewmodel/anyvalue_viewmodel.h>

#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <QTreeView>

namespace sup::gui
{

TreeViewComponentProvider::TreeViewComponentProvider(mvvm::ISessionModel *model, QTreeView *view)
    : mvvm::ItemViewComponentProvider(std::make_unique<AnyValueViewModel>(model), view)
{
  auto proxy_model = std::make_unique<mvvm::FilterNameViewModel>();
  m_filter_proxy_model = proxy_model.get();
  AddProxyModel(std::move(proxy_model));
}

void TreeViewComponentProvider::SetFilterPattern(const QString &pattern)
{
  m_filter_proxy_model->SetPattern(pattern);
}

}  // namespace sup::gui
