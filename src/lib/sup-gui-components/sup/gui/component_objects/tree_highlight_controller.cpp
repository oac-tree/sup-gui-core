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

#include "tree_highlight_controller.h"

#include "diff_highlight_controller.h"

#include <QAbstractItemModel>
#include <algorithm>

namespace sup::gui
{

TreeHighlightController::TreeHighlightController(QObject* parent_object) : QObject(parent_object) {}

TreeHighlightController::~TreeHighlightController() = default;

void TreeHighlightController::AddViewModel(QAbstractItemModel* model)
{
  if ((model == nullptr)
      || (std::find(m_view_models.begin(), m_view_models.end(), model) != m_view_models.end()))
  {
    return;
  }

  m_view_models.push_back(model);

  // Remove from the group automatically when the model is destroyed.
  connect(model, &QObject::destroyed, this,
          [this, model]()
          {
            m_view_models.erase(std::remove(m_view_models.begin(), m_view_models.end(), model),
                                m_view_models.end());
            if (m_reference_model == model)
            {
              m_reference_model = nullptr;
            }
            RecreateDiffControllers();
          });

  RecreateDiffControllers();
}

void TreeHighlightController::RemoveViewModel(QAbstractItemModel* model)
{
  auto it = std::find(m_view_models.begin(), m_view_models.end(), model);
  if (it == m_view_models.end())
  {
    return;
  }

  m_view_models.erase(it);
  disconnect(model, nullptr, this, nullptr);

  if (m_reference_model == model)
  {
    m_reference_model = nullptr;
  }

  RecreateDiffControllers();
}

void TreeHighlightController::SetReferenceViewModel(QAbstractItemModel* model)
{
  m_reference_model = model;
  RecreateDiffControllers();
}

void TreeHighlightController::SetActive(bool value)
{
  m_is_active = value;
  for (auto& controller : m_diff_controllers)
  {
    controller->SetActive(value);
  }
}

bool TreeHighlightController::IsActive() const
{
  return m_is_active;
}

void TreeHighlightController::RecreateDiffControllers()
{
  m_diff_controllers.clear();

  if (m_reference_model == nullptr)
  {
    return;
  }

  for (auto* model : m_view_models)
  {
    if (model == m_reference_model)
    {
      continue;
    }

    auto controller = std::make_unique<DiffHighlightController>(m_reference_model, model);
    controller->SetActive(m_is_active);
    controller->ProcessAll();
    m_diff_controllers.push_back(std::move(controller));
  }
}

}  // namespace sup::gui
