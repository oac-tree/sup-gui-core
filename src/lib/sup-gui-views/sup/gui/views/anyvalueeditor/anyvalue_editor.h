/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
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

/**
 * @brief The AnyValueEditor class is an evelop to provide AnyValueEditorWidget with the model.
 */
class AnyValueEditor : public AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueEditor(QWidget* parent = nullptr);
  ~AnyValueEditor() override;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() override;

  AnyValueItem* GetTopItem();

private:
  std::unique_ptr<mvvm::ApplicationModel> m_model;
  AnyValueEditorWidget* m_editor_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_
