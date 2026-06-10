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

class CustomSplitter;
class DtoExplorerPanel;
class SplittableEditorWidget;
class ConfigurationModel;

class DtoCompareView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoCompareView(QWidget* parent_widget = nullptr);
  ~DtoCompareView() override;

  DtoCompareView(const DtoCompareView&) = delete;
  DtoCompareView& operator=(const DtoCompareView&) = delete;
  DtoCompareView(DtoCompareView&&) noexcept = delete;
  DtoCompareView& operator=(DtoCompareView&&) noexcept  = delete;

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void SetConfigurationModel(ConfigurationModel* model);

private:
  void OnImportFromFileRequest(const QString& file_name);

  void ReadSettings();
  void WriteSettings();

  CustomSplitter* m_splitter{nullptr};
  DtoExplorerPanel* m_explorer_panel{nullptr};
  SplittableEditorWidget* m_splittable_config_editor{nullptr};

  ConfigurationModel* m_configuration_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_VIEW_H_
