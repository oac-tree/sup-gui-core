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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_ACTIONS_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>

class QAction;

namespace sup::gui
{

class WaveformEditorActionHandler;

/**
 * @brief The WaveformEditorActions class defines actions related to the editing of a single
 * waveform in the context of WaveformEditorWidget.
 */
class WaveformEditorActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey
  {
    kPointer,
    kPan,
    kZoomIn,
    kZoomOut,
    kCenterCanvas,
    kAddColumnBefore,
    kAddColumnAfter,
    kRemoveColumn,
    kTotalCount
  };

  explicit WaveformEditorActions(WaveformEditorActionHandler* action_handler,
                                 QObject* parent = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

signals:
  void ZoomInRequest();
  void ZoomOutRequest();
  void SetViewportToContentRequest();

private:
  /**
   * @brief Creates all actions.
   */
  void SetupActions();

  QAction* m_zoom_in{nullptr};
  QAction* m_zoom_out{nullptr};
  QAction* m_center_canvas{nullptr};
  QAction* m_add_column_before{nullptr};
  QAction* m_add_column_after{nullptr};
  QAction* m_remove_column{nullptr};

  WaveformEditorActionHandler* m_action_handler{nullptr};
  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_ACTIONS_H_
