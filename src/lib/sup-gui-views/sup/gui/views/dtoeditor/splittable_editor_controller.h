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
#include <memory>

class QSplitter;

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
public:
  /**
   * @brief Main c-tor
   *
   * @param splitter External splitter widget, where the panels will be added/removed.
   */
  explicit SplittableEditorController(QSplitter* splitter, QObject* parent_object = nullptr);

  /**
   * @brief Adds a new widget to the splitter.
   *
   * @param after_widget If nullptr, the new widget is added at the end. Otherwise, it is added
   * after the specified widget.
   */
  QWidget* CreatePanel(QWidget* after_widget = nullptr);

  /**
   * @brief Closes and deletes the specified widget in the splitter.
   */
  void ClosePanel(QWidget* widget_to_close);

  /**
   * @brief Returns the widget currently in focus.
   */
  QWidget* GetFocusWidget();

private:
  virtual std::unique_ptr<SplittableComboPanel> CreateSplittableComboPanel();

  QSplitter* m_splitter{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_CONTROLLER_H_
