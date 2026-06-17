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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_PANEL_MIRROR_CONTROLLER_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_PANEL_MIRROR_CONTROLLER_H_

#include <QObject>
#include <memory>

namespace sup::gui
{

class SplittableComboPanel;
class TabWidgetController;

/**
 * @brief The ConfigurationPanelMirrorController class envelops all controllers that synchronise
 * the appearance of SplittableComboPanel instances.
 *
 * Its purpose is to keep the look of all panels in sync (e.g. the currently selected tab).
 */
class ConfigurationPanelMirrorController : public QObject
{
  Q_OBJECT

public:
  explicit ConfigurationPanelMirrorController(QObject* parent_object = nullptr);
  ~ConfigurationPanelMirrorController() override;

  ConfigurationPanelMirrorController(const ConfigurationPanelMirrorController&) = delete;
  ConfigurationPanelMirrorController& operator=(const ConfigurationPanelMirrorController&) = delete;
  ConfigurationPanelMirrorController(ConfigurationPanelMirrorController&&) noexcept = delete;
  ConfigurationPanelMirrorController& operator=(ConfigurationPanelMirrorController&&) noexcept =
      delete;

  /**
   * @brief Sets active flag to a give value.
   *
   * All controllers that synchronise the appearance of SplittableComboPanel instances will be
   * activated or deactivated.
   */
  void SetActive(bool value);

  /**
   * @brief Registers a newly added panel with the underlying synchronisation controllers.
   *
   * @note The actual registration logic will be implemented in the next step.
   */
  void ProcessPanelAdd(sup::gui::SplittableComboPanel* panel);

  /**
   * @brief Unregisters a panel that is about to be closed from the underlying synchronisation
   * controllers.
   *
   * @note The actual unregistration logic will be implemented in the next step.
   */
  void ProcessPanelRemove(sup::gui::SplittableComboPanel* panel);

  /**
   * @brief Returns the underlying tab widget controller.
   *
   * Used for testing.
   */
  TabWidgetController* GetTabWidgetController() const;

private:
  std::unique_ptr<TabWidgetController> m_tab_widget_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_PANEL_MIRROR_CONTROLLER_H_
