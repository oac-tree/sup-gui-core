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

#ifndef SUP_GUI_ANYVALUEEDITOR_TREE_VIEW_COMPONENT_PROVIDER_H_
#define SUP_GUI_ANYVALUEEDITOR_TREE_VIEW_COMPONENT_PROVIDER_H_

#include <QList>
#include <QObject>
#include <memory>
#include <vector>

class QTreeView;
class QItemSelectionModel;
class QModelIndex;
class QAbstractProxyModel;

namespace mvvm
{
class ApplicationModel;
class ItemViewComponentProvider;
class SessionItem;
class ViewModelDelegate;
class ViewModel;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueViewModel;
class AnyValueFilteredViewModel;
class AnyValueItem;

/**
 * @brief The TreeViewComponentProvider class provides QAbstractItemView with custom components:
 * viemodel, delegate and selection model.
 *
 * @details Provider owns components but doesn't own a view. This provider is oriented for
 * AnyValueItem editing. Internally it uses AnyValueViewModel to have 3-column AnyValue tree and
 * special AnyValueFilteredViewModel to allow filtering.
 */

class TreeViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  explicit TreeViewComponentProvider(mvvm::ApplicationModel* model, QTreeView* view);
  ~TreeViewComponentProvider() override;

  /**
   * @brief Set an item to be a new invisible root item for view model.
   */
  void SetItem(mvvm::SessionItem* item);

  void SetFilterPattern(const QString& pattern);

  const mvvm::SessionItem* GetSelectedItem() const;
  std::vector<const mvvm::SessionItem*> GetSelectedItems() const;

  void SetSelectedItem(const mvvm::SessionItem* item);
  void SetSelectedItems(const std::vector<const mvvm::SessionItem*>& items);

  /**
   * @brief Returns view selection model.
   */
  QItemSelectionModel* GetSelectionModel() const;

  /**
   * @brief Returns view model.
   */
  const mvvm::ViewModel* GetViewModel() const;

  /**
   * @brief Returns underlying proxy model.
   */
  const QAbstractProxyModel* GetProxyModel() const;

  /**
   * @brief Returns item from view index.
   *
   * @details The method takes into account the existence of proxy model. When proxy model is set,
   * the result is obtained by applying QAbstractItemProxyModel::mapToSource to given index, and
   * then getting item out of it.
   */
  const mvvm::SessionItem* GetItemFromViewIndex(const QModelIndex& index) const;

  /**
   * @brief Returns view indices for given item.
   *
   * @details View indices can be used for operation through a view. The method takes into account
   * the existence of proxy model. When proxy model is set, the result is obtained by applying
   * QAbstractItemProxyModel::mapFromSource to the original item's indices.
   */
  QList<QModelIndex> GetViewIndices(const mvvm::SessionItem* item) const;

private:
  std::unique_ptr<AnyValueViewModel> m_view_model;
  std::unique_ptr<AnyValueFilteredViewModel> m_proxy_model;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  QTreeView* m_tree_view{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_TREE_VIEW_COMPONENT_PROVIDER_H_
