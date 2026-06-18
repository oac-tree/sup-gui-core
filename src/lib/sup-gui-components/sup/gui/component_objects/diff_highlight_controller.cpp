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

#include "diff_highlight_controller.h"

#include <QAbstractItemModel>
#include <QColor>
#include <algorithm>
#include <vector>

namespace
{

//! A single level of an index path: the row and column within the parent.
struct PathItem
{
  int row{0};
  int column{0};
};

//! Returns the path of an index as a sequence of (row, column) from the root (top-most first).
//! By Qt convention all ancestor indices have column 0; only the leaf carries the real column.
std::vector<PathItem> GetIndexPath(QModelIndex index)
{
  std::vector<PathItem> path;
  for (; index.isValid(); index = index.parent())
  {
    path.push_back({index.row(), index.column()});
  }
  std::reverse(path.begin(), path.end());
  return path;
}

//! Reconstructs an index in the given model from a path, or returns an invalid index if the path
//! does not exist in the model.
QModelIndex GetIndexFromPath(const QAbstractItemModel* model, const std::vector<PathItem>& path)
{
  QModelIndex index;  // start from the root
  for (const auto& item : path)
  {
    index = model->index(item.row, item.column, index);
    if (!index.isValid())
    {
      return {};
    }
  }
  return index;
}

//! Returns true when the changed roles affect the display value (an empty list means all roles).
bool AffectsDisplayRole(const QVector<int>& roles)
{
  return roles.isEmpty() || roles.contains(Qt::DisplayRole);
}

}  // namespace

namespace sup::gui
{

DiffHighlightController::DiffHighlightController(QAbstractItemModel* reference_model,
                                                 QAbstractItemModel* highlight_model,
                                                 QObject* parent_object)
    : QObject(parent_object), m_reference_model(reference_model), m_highlight_model(highlight_model)
{
  if (m_reference_model != nullptr)
  {
    connect(m_reference_model, &QAbstractItemModel::dataChanged, this,
            [this](const QModelIndex& top_left, const QModelIndex& bottom_right,
                   const QVector<int>& roles)
            {
              if (AffectsDisplayRole(roles))
              {
                ProcessReferenceChange(top_left, bottom_right);
              }
            });
    connect(m_reference_model, &QObject::destroyed, this,
            [this]() { m_reference_model = nullptr; });
  }

  if (m_highlight_model != nullptr)
  {
    connect(m_highlight_model, &QAbstractItemModel::dataChanged, this,
            [this](const QModelIndex& top_left, const QModelIndex& bottom_right,
                   const QVector<int>& roles)
            {
              if (AffectsDisplayRole(roles))
              {
                ProcessHighlightChange(top_left, bottom_right);
              }
            });
    connect(m_highlight_model, &QObject::destroyed, this,
            [this]() { m_highlight_model = nullptr; });
  }
}

DiffHighlightController::~DiffHighlightController() = default;

void DiffHighlightController::SetActive(bool value)
{
  m_is_active = value;
}

bool DiffHighlightController::IsActive() const
{
  return m_is_active;
}

void DiffHighlightController::ProcessReferenceChange(const QModelIndex& top_left,
                                                     const QModelIndex& bottom_right)
{
  if ((!m_is_active) || m_is_updating || (m_highlight_model == nullptr))
  {
    return;
  }

  const QModelIndex parent = top_left.parent();
  for (int row = top_left.row(); row <= bottom_right.row(); ++row)
  {
    for (int column = top_left.column(); column <= bottom_right.column(); ++column)
    {
      const QModelIndex reference_index = m_reference_model->index(row, column, parent);
      const QModelIndex highlight_index =
          GetIndexFromPath(m_highlight_model, GetIndexPath(reference_index));
      UpdateHighlightCell(reference_index, highlight_index);
    }
  }
}

void DiffHighlightController::ProcessHighlightChange(const QModelIndex& top_left,
                                                     const QModelIndex& bottom_right)
{
  if ((!m_is_active) || m_is_updating || (m_reference_model == nullptr))
  {
    return;
  }

  const QModelIndex parent = top_left.parent();
  for (int row = top_left.row(); row <= bottom_right.row(); ++row)
  {
    for (int column = top_left.column(); column <= bottom_right.column(); ++column)
    {
      const QModelIndex highlight_index = m_highlight_model->index(row, column, parent);
      const QModelIndex reference_index =
          GetIndexFromPath(m_reference_model, GetIndexPath(highlight_index));
      UpdateHighlightCell(reference_index, highlight_index);
    }
  }
}

void DiffHighlightController::UpdateHighlightCell(const QModelIndex& reference_index,
                                                  const QModelIndex& highlight_index)
{
  if ((!reference_index.isValid()) || (!highlight_index.isValid()))
  {
    return;
  }

  const bool is_different =
      (reference_index.data(Qt::DisplayRole) != highlight_index.data(Qt::DisplayRole));
  const QVariant background = is_different ? QVariant(QColor(Qt::red)) : QVariant();

  m_is_updating = true;
  m_highlight_model->setData(highlight_index, background, Qt::BackgroundRole);
  m_is_updating = false;
}

}  // namespace sup::gui
