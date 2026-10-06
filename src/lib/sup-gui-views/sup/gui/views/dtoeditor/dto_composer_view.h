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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_

#include <QWidget>

namespace sup::gui
{

class IAppCommandService;
class SettingsSplitter;
struct AppViewContext;
class DtoComposerToolsPanel;
class AnyValueEditorWidget;
class ConfigurationModel;

/**
 * @brief The DtoComposerView class represents a main tab-based view to assemble AnyValue.
 *
 * Alternative to DtoComposerTabView (to decide, which one will remain)
 *
 * It contains tabs with multiple instances of AnyValueEditor, belongs to MainWindow.
 */
class DtoComposerView : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The services of the context must outlive the view.
   */
  explicit DtoComposerView(const AppViewContext& context, QWidget* parent_widget = nullptr);
  ~DtoComposerView() override;

  DtoComposerView(const DtoComposerView&) = delete;
  DtoComposerView& operator=(const DtoComposerView&) = delete;
  DtoComposerView(DtoComposerView&&) = delete;
  DtoComposerView& operator=(DtoComposerView&&) = delete;

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void SetConfigurationModel(ConfigurationModel* model);

private:
  void SetupConnections();
  void SummonContextMenu(const QPoint& point);

  IAppCommandService& m_command_service;

  SettingsSplitter* m_splitter{nullptr};
  DtoComposerToolsPanel* m_tools_panel{nullptr};
  AnyValueEditorWidget* m_anyvalue_editor{nullptr};
  ConfigurationModel* m_configuration_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_
