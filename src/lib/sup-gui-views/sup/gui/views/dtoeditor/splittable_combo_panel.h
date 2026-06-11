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
class ISessionModel;
class ModelListener;
class SessionItem;
}  // namespace mvvm

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
 * - toolbar with a combo selector to select between various items to edit
 * - tool buttons to request the split of the view, or current view close
 * - an instance of concrete item editor
 * - a placeholder to show instead of the editor when no item is set
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

  /**
   * @brief Sets the model for this editor.
   */
  void SetModel(ConfigurationModel* model);

  /**
   * @brief Sets the item to be edited.
   */
  void SetItem(mvvm::SessionItem* configuration_item);

  /**
   * @brief Gets the currently edited item.
   */
  mvvm::SessionItem* GetCurrentItem();

  std::vector<ConfigurationItem*> GetAvailableConfigurations() const;

  /**
   * @brief Shows (paints) this editor as active.
   *
   * Active editor normally has a toolbar with the green line at the bottom. It is the one where the
   * user clicked last.
   */
  void ShowAsActive(bool value);

  /**
   * @brief Shows (paints) this editor as the last editor in the splitter.
   *
   * The last editor might have different appearance (close button is disabled).
   */
  void ShowAsLastEditor(bool value);

signals:
  void splitViewRequest();
  void closeViewRequest();
  void itemSelectionChanged(mvvm::ItemSelection selection);

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
   * @brief Creates a callback to get list of possible item names for combo toolbar.
   */
  std::function<QList<QString>()> CreateItemNamesCallback();

  ComboToolBar* m_tool_bar{nullptr};
  QStackedWidget* m_stacked_widget{nullptr};
  PlaceholderWidget* m_placeholder_widget{nullptr};
  ConfigTabWidget* m_config_widget{nullptr};

  mvvm::SessionItem* m_current_item{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
  ConfigurationModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_SPLITTABLE_COMBO_PANEL_H_
