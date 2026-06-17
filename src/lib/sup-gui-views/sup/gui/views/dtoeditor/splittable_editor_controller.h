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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_CONTROLLER_H_
#define SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_CONTROLLER_H_

#include <QObject>
#include <functional>
#include <memory>
#include <mvvm/model/item_selection.h>

class QSplitter;

namespace mvvm {
class ISessionModel;
}

namespace sup::gui
{

class SplittableComboPanel;

template <typename WidgetT>
class WidgetFocusHandler;

/**
 * @brief The SplittableEditorController class holds the main logic of SplittableEditorWidget.
 *
 * - controls adding/removal of vertical panels to external QSplitter
 * - handles split-panel and close-panel requests
 * - controls which widget is in focus, and update its visual properties accordingly
 */
class SplittableEditorController : public QObject
{
  Q_OBJECT

public:
  using create_panel_func_t = std::function<std::unique_ptr<SplittableComboPanel>()>;

  /**
   * @brief Main c-tor
   *
   * @param splitter External splitter widget, where the panels will be added/removed.
   * @param factory_func Factory function to create new panels when requested.
   * @param parent_object Optional parent object for Qt ownership.
   */
  explicit SplittableEditorController(QSplitter* splitter, create_panel_func_t factory_func,
                                      QObject* parent_object = nullptr);
  ~SplittableEditorController() override;

  SplittableEditorController(const SplittableEditorController&) = delete;
  SplittableEditorController& operator=(const SplittableEditorController&) = delete;
  SplittableEditorController(SplittableEditorController&&) noexcept = delete;
  SplittableEditorController& operator=(SplittableEditorController&&) noexcept = delete;

  /**
   * @brief Sets the model to all editors.
   */
  void SetModel(mvvm::ISessionModel* model);

  /**
   * @brief Adds a new widget to the splitter.
   *
   * @param after_widget If nullptr, the new widget is added at the end. Otherwise, it is added
   * after the specified widget.
   */
  SplittableComboPanel* CreatePanel(SplittableComboPanel* after_widget = nullptr);

  /**
   * @brief Closes and deletes the specified widget in the splitter.
   *
   * The widget first will be hidden, and deleted later, when the event loop will process the
   * deletion event.
   */
  void ClosePanel(SplittableComboPanel* widget_to_close);

  /**
   * @brief Returns the widget currently in focus.
   */
  SplittableComboPanel* GetFocusWidget();

  /**
   * @brief Sets the focus to the specified widget (should be already added to the splitter).
   */
  void SetFocusWidget(SplittableComboPanel* widget);

  /**
   * @brief Returns list of panels currently in the editor.
   *
   * Used for testing.
   */
  std::vector<SplittableComboPanel*> GetPanels() const;

signals:
  //! The signal is emitted when user changes the selection in the combo box of any panel.
  //! It is used to propagate the selection back to list view.
  void focusPanelSelectionChanged(const mvvm::ItemSelection& selection);

  //! The signal is emitted right after a new panel has been added to the splitter.
  void newPanelAdded(sup::gui::SplittableComboPanel* panel);

  //! The signal is emitted just before a panel is closed and scheduled for deletion.
  void aboutToClosePanel(sup::gui::SplittableComboPanel* panel);

private:
  std::unique_ptr<SplittableComboPanel> CreateAndConnectSplittableComboPanel();
  std::unique_ptr<SplittableComboPanel> CreateSplittableComboPanel();

  void onSplitPanelRequest();
  void onClosePanelRequest();
  void onFocusPanelRequest();
  void onItemSelectionChanged(const mvvm::ItemSelection& selection);

  QSplitter* m_splitter{nullptr};
  create_panel_func_t m_factory_func;
  std::unique_ptr<WidgetFocusHandler<SplittableComboPanel>> m_focus_handler;
  mvvm::ItemSelection m_previous_selection_in_focus_widget;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_CONTROLLER_H_
