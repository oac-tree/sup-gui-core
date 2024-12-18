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

#include "dto_waveform_actions.h"

#include <sup/gui/components/dto_waveform_action_handler.h>
#include <sup/gui/style/style_helper.h>

namespace sup::gui
{

DtoWaveformActions::DtoWaveformActions(DtoWaveformActionHandler *action_handler, QObject *parent)
    : QObject(parent), m_action_handler(action_handler)
{
  SetupActions();
}

QList<QAction *> DtoWaveformActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void DtoWaveformActions::SetupActions()
{
  // add waveform
  m_add_waveform_action = new QAction("Add", this);
  m_add_waveform_action->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  m_add_waveform_action->setToolTip(
      "Creates new empty waveform.\n"
      "Waveform will be added to the list after current selection");
  connect(m_add_waveform_action, &QAction::triggered, this,
          [this]() { m_action_handler->AddWaveform(); });
  m_action_map.Add(ActionKey::kAddWaveform, m_add_waveform_action);

  // remove waveform
  m_remove_waveform_action = new QAction("Remove", this);
  m_remove_waveform_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_waveform_action->setToolTip("Remove currently selected waveform");
  connect(m_remove_waveform_action, &QAction::triggered, this,
          [this]() { m_action_handler->RemoveWaveform(); });
  m_action_map.Add(ActionKey::kRemoveWaveform, m_remove_waveform_action);
}

}  // namespace sup::gui
