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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_COMBO_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_COMBO_PANEL_H_

#include <mvvm/model/item_selection.h>

#include <QWidget>
#include <functional>

class QStackedWidget;

namespace mvvm
{
class ModelListener;
}

namespace sup::gui
{

class ComboToolBar;
class PlaceholderWidget;
class ConfigTabWidget;
class ConfigurationItem;
class ConfigurationModel;

/**
 * @brief The SplittableComboPanel class contains an item editor with the possibility to switch
 * between different items.
 *
 * It is a part of SplittableEditorWidget.
 *
 * The widget contains
 * - toolbar with a combo selector to select between various configurations
 * - tool buttons to request the split of the view, or current view close
 * - an instance of concrete item editor
 * - a placeholder to show when no configuration is opened.
 */
class SplittableComboPanel : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief The PlaceholderMode enum defines modes of the placeholder widget.
   *
   * Placeholder hides the main editor when it is not ready.
   */
  enum class PlaceholderMode : std::uint8_t
  {
    kShowPlaceholder,
    kShowEditor
  };

  explicit SplittableComboPanel(QWidget* parent_widget = nullptr);

  ~SplittableComboPanel() override;

  SplittableComboPanel(const SplittableComboPanel& other) = delete;
  SplittableComboPanel& operator=(const SplittableComboPanel&) = delete;
  SplittableComboPanel(SplittableComboPanel&&) = delete;
  SplittableComboPanel& operator=(SplittableComboPanel&&) = delete;

  ConfigurationModel* GetModel();

  void SetModel(ConfigurationModel* model);

  /**
   * @brief Sets the configuration to be edited.
   */
  void SetConfiguration(ConfigurationItem* configuration_item);

  /**
   * @brief Gets the currently edited configuration.
   */
  ConfigurationItem* GetCurrentConfiguration();

  std::vector<ConfigurationItem*> GetAvailableConfigurations() const;

signals:
  void splitViewRequest();
  void closeViewRequest();
  void selectedConfigurationChanged(mvvm::ItemSelection selection);

private:
  /**
   * @brief Sets the placeholder mode.
   *
   * Placeholders hides the main editor when it is not ready.
   */
  void SetPlaceholderMode(PlaceholderMode mode);

  void SetupConnections();

  /**
   * @brief Setup model listener to track when configuration us renamed, or deleted.
   */
  void SetupModelListener();

  /**
   * @brief Show given configuration in ConfigTabWidget.
   */
  void ShowConfigurationInEditor(ConfigurationItem* item);

  /**
   * @brief Creates a callback to get list of configuration names for combo toolbar.
   */
  std::function<QList<QString>()> CreateConfigurationNamesCallback();

  ComboToolBar* m_tool_bar{nullptr};
  QStackedWidget* m_stacked_widget{nullptr};
  PlaceholderWidget* m_placeholder_widget{nullptr};
  ConfigTabWidget* m_config_widget{nullptr};
  ConfigurationItem* m_current_configuration_item{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
  ConfigurationModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_COMBO_PANEL_H_
