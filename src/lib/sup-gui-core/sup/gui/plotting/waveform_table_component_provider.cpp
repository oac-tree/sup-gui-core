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

#include "waveform_table_component_provider.h"

#include <sup/gui/plotting/waveform_twocolumn_viewmodel.h>

#include <mvvm/delegates/viewmodel_delegate.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <QHeaderView>
#include <QTableView>
#include <QTransposeProxyModel>

namespace sup::gui
{

WaveformTableComponentProvider::WaveformTableComponentProvider(mvvm::ApplicationModel *model,
                                                               QTableView *view)
    : m_table_view(view)
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
    , m_horizontal_proxy_model(new QTransposeProxyModel(this))
{
  SetModel(model);
}

WaveformTableComponentProvider::~WaveformTableComponentProvider() = default;

void WaveformTableComponentProvider::SetModel(mvvm::ApplicationModel *model)
{
  m_model = model;
    m_table_view_model = std::make_unique<sup::gui::WaveformTwoColumnViewModel>(model);
  m_horizontal_proxy_model->setSourceModel(m_table_view_model.get());
  m_table_view->setModel(m_horizontal_proxy_model);
  m_table_view->setItemDelegate(m_delegate.get());

  connect(m_table_view_model.get(), &mvvm::ViewModel::modelAboutToBeReset, this,
          &WaveformTableComponentProvider::OnViewModelReset, Qt::UniqueConnection);
}

void WaveformTableComponentProvider::SetItem(mvvm::SessionItem *item)
{
  m_table_view_model->SetRootSessionItem(item);
}

QItemSelectionModel *WaveformTableComponentProvider::GetSelectionModel() const
{
  return m_table_view->selectionModel();
}

const mvvm::ViewModel *WaveformTableComponentProvider::GetViewModel() const
{
  return m_table_view_model.get();
}

const QAbstractProxyModel *WaveformTableComponentProvider::GetProxyModel() const
{
  return m_horizontal_proxy_model;
}

const mvvm::SessionItem *WaveformTableComponentProvider::GetItemFromViewIndex(
    const QModelIndex &index) const
{
  auto source_index = m_horizontal_proxy_model->mapToSource(index);
  return GetViewModel()->GetSessionItemFromIndex(source_index);
}

QList<QModelIndex> WaveformTableComponentProvider::GetViewIndices(
    const mvvm::SessionItem *item) const
{
  const auto source_indices = GetViewModel()->GetIndexOfSessionItem(item);

  auto on_index = [this](auto index) { return m_horizontal_proxy_model->mapFromSource(index); };
  QList<QModelIndex> result;
  std::transform(std::begin(source_indices), std::end(source_indices), std::back_inserter(result),
                 on_index);

  return result;
}

int WaveformTableComponentProvider::GetSelectedPointIndex()
{
  auto item = GetSelectedItem();
  // returning parent's index
  return item ? item->GetParent()->GetTagIndex().index : -1;
}

const mvvm::SessionItem *WaveformTableComponentProvider::GetSelectedItem() const
{
  auto selected = GetSelectedItems();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<const mvvm::SessionItem *> WaveformTableComponentProvider::GetSelectedItems() const
{
  std::vector<const mvvm::SessionItem *> result;

  for (auto index : GetSelectionModel()->selectedIndexes())
  {
    // skipping nullptr
    if (auto item = GetItemFromViewIndex(index); item)
    {
      result.push_back(item);
    }
  }

  return mvvm::utils::UniqueWithOrder(result);
}

void WaveformTableComponentProvider::SetSelectedItem(const mvvm::SessionItem *item)
{
  SetSelectedItems({item});
}

void WaveformTableComponentProvider::SetSelectedItems(
    const std::vector<const mvvm::SessionItem *> &items)
{
  GetSelectionModel()->clearSelection();
  QItemSelection selection;
  for (auto item : items)
  {
    for (auto index : GetViewIndices(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }

  //  auto flags = QItemSelectionModel::Select;  // not clear, which one to use
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Columns;
  GetSelectionModel()->select(selection, flags);
}

void WaveformTableComponentProvider::OnViewModelReset()
{
  GetSelectionModel()->clearSelection();
}

}  // namespace pspsdemo
