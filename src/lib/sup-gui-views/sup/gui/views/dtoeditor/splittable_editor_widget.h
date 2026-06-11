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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QSplitter;

namespace sup::gui
{

class SplittableComboPanel;
class ConfigurationModel;
class SplittableEditorController;

/**
 * @brief The SplittableEditorWidget class is a complex editor with the possibility to split
 * the area.
 *
 * Each area contains a vertical panel with the selector of one of the opened documents.
 */
class SplittableEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SplittableEditorWidget(QWidget* parent_widget = nullptr);

  ~SplittableEditorWidget() override;

  SplittableEditorWidget(const SplittableEditorWidget&) = delete;
  SplittableEditorWidget& operator=(const SplittableEditorWidget&) = delete;
  SplittableEditorWidget(SplittableEditorWidget&&) noexcept = delete;
  SplittableEditorWidget& operator=(SplittableEditorWidget&&) noexcept = delete;

  void SetModel(ConfigurationModel* model);

  /**
   * @brief Adds panel to the splitter.
   */
  void CreatePanel();

private:
  std::unique_ptr<SplittableEditorController> CreateSplitterController() const;

  QSplitter* m_splitter{nullptr};
  std::unique_ptr<SplittableEditorController> m_splitter_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_EDITOR_WIDGET_H_
