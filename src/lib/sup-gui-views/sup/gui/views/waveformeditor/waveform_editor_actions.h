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
class QWidgetAction;
class QButtonGroup;
class QToolButton;

namespace sup::gui
{

class WaveformEditorActionHandler;
class ActionMenu;

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
    kMoreSettings,
    kTotalCount
  };

  explicit WaveformEditorActions(WaveformEditorActionHandler* action_handler,
                                 QObject* parent = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Set active button ID for pointer group.
   *
   * button_id=0 - button "select" will appear pressed, the button "Pan" released
   * button_id=1 - button "select" will appear released, the button "Pan" pressed
   */
  void SetPointerButtonGroup(int button_id);

signals:
  void ZoomInRequest();
  void ZoomOutRequest();
  void SetViewportToContentRequest();
  int ChangeOperationModeRequest(int);

private:
  /**
   * @brief Setup actions related to canvas.
   */
  void SetupCanvasActions();

  /**
   * @brief Setup actions related to table manipulation.
   */
  void SetupTableActions();

  QButtonGroup* m_pointer_button_group{nullptr};
  QToolButton* m_pointer_button{nullptr};
  QWidgetAction* m_pointer_action{nullptr};
  QToolButton* m_pan_button{nullptr};
  QWidgetAction* m_pan_action{nullptr};

  QAction* m_zoom_in_action{nullptr};
  QAction* m_zoom_out_action{nullptr};
  QAction* m_center_canvas_action{nullptr};

  ActionMenu* m_more_settings_action{nullptr};

  QAction* m_add_column_before{nullptr};
  QAction* m_add_column_after{nullptr};
  QAction* m_remove_column{nullptr};

  WaveformEditorActionHandler* m_action_handler{nullptr};
  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_ACTIONS_H_
