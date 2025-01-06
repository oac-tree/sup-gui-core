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

#ifndef SUP_GUI_VIEWMODEL_TREE_VIEW_COMPONENT_PROVIDER_H_
#define SUP_GUI_VIEWMODEL_TREE_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/providers/item_view_component_provider.h>

class QTreeView;

namespace mvvm
{
class FilterNameViewModel;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The TreeViewComponentProvider class provides QAbstractItemView with custom components:
 * viemodel, delegate and selection model.
 *
 * This provider is oriented for AnyValueItem editing. Internally it uses AnyValueViewModel to have
 * 3-column AnyValue tree and special AnyValueFilteredViewModel to allow filtering.
 */
class TreeViewComponentProvider : public mvvm::ItemViewComponentProvider
{
  Q_OBJECT

public:
  explicit TreeViewComponentProvider(mvvm::ISessionModel* model, QTreeView* view);

  /**
   * @brief Sets filtering pattern to the display name in the first column of AnyValueItem tree.
   *
   * Item will be filtered out if its display name doesn't contain a given pattern (case
   * insensitive).
   */
  void SetFilterPattern(const QString& pattern);

private:
  mvvm::FilterNameViewModel* m_filter_proxy_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWMODEL_TREE_VIEW_COMPONENT_PROVIDER_H_
