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

#ifndef SUP_GUI_PLOTTING_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_
#define SUP_GUI_PLOTTING_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_

#include <QWidget>
#include <memory>

class QTableView;
class QTransposeProxyModel;
class QAbstractProxyModel;
class QItemSelectionModel;

namespace mvvm
{
class ApplicationModel;
class SessionItem;
class ViewModel;
class ViewModelDelegate;
}  // namespace mvvm

namespace sup::gui
{

class WaveformTwoColumnViewModel;

/**
 * @brief The WaveformTableComponentProvider class provides table view with custom viewmodel, proxy
 * model and selection logic.
 *
 * @details It is intended for presenting a single waveform as a horizontal table with two rows
 * corresponding to (x,y) values.
 */

class WaveformTableComponentProvider : public QObject
{
  Q_OBJECT

public:
  explicit WaveformTableComponentProvider(mvvm::ApplicationModel* model, QTableView* view);
  ~WaveformTableComponentProvider() override;

  void SetModel(mvvm::ApplicationModel* model);

  void SetItem(mvvm::SessionItem* item);

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

  /**
   * @brief Returns index of selected column.
   */
  int GetSelectedPointIndex();

  const mvvm::SessionItem* GetSelectedItem() const;
  std::vector<const mvvm::SessionItem*> GetSelectedItems() const;

  /**
   * @brief Set item selected in table widget.
   */
  void SetSelectedItem(const mvvm::SessionItem* item);

  void SetSelectedItems(const std::vector<const mvvm::SessionItem*>& items);

private:
  void OnViewModelReset();

  QTableView* m_table_view{nullptr};

  mvvm::ApplicationModel* m_model{nullptr};
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  std::unique_ptr<WaveformTwoColumnViewModel> m_table_view_model;
  QTransposeProxyModel* m_horizontal_proxy_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_WAVEFORM_TABLE_COMPONENT_PROVIDER_H_
