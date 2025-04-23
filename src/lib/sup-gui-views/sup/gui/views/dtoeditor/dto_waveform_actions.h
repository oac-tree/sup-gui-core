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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_ACTIONS_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>

class QAction;

namespace sup::gui
{

class DtoWaveformActionHandler;

/**
 * @brief The DtoWaveformActions class contains the main actions of DtoWaveformView.
 *
 * These are actions releated to the creation and removal of waveforms from the list, as well s
 * cut/copy/paste operations.
 */
class DtoWaveformActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey
  {
    kAddWaveform,
    kRemoveWaveform,
    kTotalCount
  };

  explicit DtoWaveformActions(DtoWaveformActionHandler* action_handler, QObject* parent = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

private:
  void SetupActions();

  QAction* m_add_waveform_action{nullptr};
  QAction* m_remove_waveform_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
  DtoWaveformActionHandler* m_action_handler{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_ACTIONS_H_
