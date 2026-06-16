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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_VIEW_H_

#include <QWidget>

namespace sup::gui
{

class CodeView;
class CustomSplitter;
class DtoExplorerPanel;
class ItemStackWidget;
class ConfigurationModel;

/**
 * @brief The DtoExplorerView is a main widget to browse configurations on disk.
 *
 * Contains a file browser and list of opened configurations on the left, and JSON viewer on the
 * right.
 *
 * It is a first main view of DtoEditorMainWindow.
 */
class DtoExplorerView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoExplorerView(QWidget* parent_widget = nullptr);
  ~DtoExplorerView() override;

  DtoExplorerView(const DtoExplorerView&) = delete;
  DtoExplorerView& operator=(const DtoExplorerView&) = delete;
  DtoExplorerView(DtoExplorerView&&) = delete;
  DtoExplorerView& operator=(DtoExplorerView&&) = delete;

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void SetConfigurationModel(ConfigurationModel* model);

  void ShowFileContent(const QString& file_name);

  void OnImportFromFileRequest(const QString& file_name);

private:
  DtoExplorerPanel* m_explorer_panel{nullptr};
  CodeView* m_xml_view{nullptr};
  ItemStackWidget* m_right_panel{nullptr};
  CustomSplitter* m_splitter{nullptr};
  ConfigurationModel* m_configuration_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_VIEW_H_
