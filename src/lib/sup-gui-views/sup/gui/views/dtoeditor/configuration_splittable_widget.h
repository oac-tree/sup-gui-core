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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_SPLITTABLE_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_SPLITTABLE_WIDGET_H_

#include <QWidget>
#include <memory>

class QSplitter;

namespace mvvm
{
class ItemSelection;
}

namespace sup::gui
{

class SplittableComboPanel;
class ConfigurationModel;
class ConfigurationItem;
class SplittableEditorController;
class ConfigurationPanelMirrorController;

/**
 * @brief The SplittableEditorWidget class is a complex editor with the possibility to split
 * the area.
 *
 * Each area contains a vertical panel with the selector of one of the opened documents.
 */
class ConfigurationSplittableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ConfigurationSplittableWidget(QWidget* parent_widget = nullptr);

  ~ConfigurationSplittableWidget() override;

  ConfigurationSplittableWidget(const ConfigurationSplittableWidget&) = delete;
  ConfigurationSplittableWidget& operator=(const ConfigurationSplittableWidget&) = delete;
  ConfigurationSplittableWidget(ConfigurationSplittableWidget&&) noexcept = delete;
  ConfigurationSplittableWidget& operator=(ConfigurationSplittableWidget&&) noexcept = delete;

  void SetModel(ConfigurationModel* model);

  void SetConfiguration(ConfigurationItem* config);

  /**
   * @brief Sets panel appearance sync flag to a given value.
   */
  void SetPanelSyncAppearance(bool value);

  /**
   * @brief Adds panel to the splitter.
   */
  void CreatePanel();

  /**
   * @brief Returns controller of the editor, which allows to manipulate panels and their content.
   *
   * Used for testing.
   */
  SplittableEditorController* GetController() const;

  /**
   * @brief Returns controller responsible for mirroring the appearance of panels.
   *
   * Used for testing.
   */
  ConfigurationPanelMirrorController* GetPanelMirrorController() const;

signals:
  void focusPanelSelectionChanged(const mvvm::ItemSelection& selection);

private:
  std::unique_ptr<SplittableEditorController> CreateSplitterController() const;

  QSplitter* m_splitter{nullptr};
  std::unique_ptr<SplittableEditorController> m_splitter_controller;
  std::unique_ptr<ConfigurationPanelMirrorController> m_panel_sync_controller;
  ConfigurationModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_SPLITTABLE_WIDGET_H_
