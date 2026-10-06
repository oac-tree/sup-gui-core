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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_VIEW_H_

#include <QWidget>

namespace sup::gui
{

class ISettingsManager;
class SettingsSplitter;
class DtoCompareToolsPanel;
class ConfigurationSplittableWidget;
class ConfigurationModel;

class DtoCompareView : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The settings manager must outlive the view.
   */
  explicit DtoCompareView(ISettingsManager& settings_manager, QWidget* parent_widget = nullptr);
  ~DtoCompareView() override;

  DtoCompareView(const DtoCompareView&) = delete;
  DtoCompareView& operator=(const DtoCompareView&) = delete;
  DtoCompareView(DtoCompareView&&) noexcept = delete;
  DtoCompareView& operator=(DtoCompareView&&) noexcept = delete;

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void SetConfigurationModel(ConfigurationModel* model);

private:
  void OnImportFromFileRequest(const QString& file_name);

  void SetupConnections();

  SettingsSplitter* m_splitter{nullptr};
  DtoCompareToolsPanel* m_explorer_panel{nullptr};
  ConfigurationSplittableWidget* m_splittable_config_editor{nullptr};

  ConfigurationModel* m_configuration_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_VIEW_H_
