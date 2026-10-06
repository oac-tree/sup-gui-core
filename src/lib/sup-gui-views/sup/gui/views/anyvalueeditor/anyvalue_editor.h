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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_

#include <sup/gui/views/anyvalueeditor/abstract_anyvalue_editor.h>

#include <QWidget>
#include <memory>

namespace mvvm
{
class ApplicationModel;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueEditorWidget;
class AnyValueItem;
class ISettingsManager;
class NullCommandService;

/**
 * @brief The AnyValueEditor class is an evelop to provide AnyValueEditorWidget with the model.
 */
class AnyValueEditor : public AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueEditor(QWidget* parent_widget = nullptr);

  /**
   * @brief Constructor storing editor settings in application settings.
   *
   * The settings manager must outlive the editor.
   */
  explicit AnyValueEditor(ISettingsManager& settings_manager, QWidget* parent_widget = nullptr);
  ~AnyValueEditor() override;

  AnyValueEditor(const AnyValueEditor&) = delete;
  AnyValueEditor& operator=(const AnyValueEditor&) = delete;
  AnyValueEditor(AnyValueEditor&&) = delete;
  AnyValueEditor& operator=(AnyValueEditor&&) = delete;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() const override;

  const AnyValueItem* GetTopItem() const;

private:
  AnyValueEditor(ISettingsManager* settings_manager, QWidget* parent_widget);

  std::unique_ptr<NullCommandService> m_command_service;
  std::unique_ptr<mvvm::ApplicationModel> m_model;
  //!< Deliberately put in unique_ptr to delete widget before NullCommandService.
  //!< Without this, it would be deleted as a layout's child at a later moment of time.
  std::unique_ptr<AnyValueEditorWidget> m_editor_widget;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_
