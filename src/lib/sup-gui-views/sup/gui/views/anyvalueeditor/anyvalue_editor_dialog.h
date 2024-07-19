/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_

#include <QDialog>
#include <memory>

namespace sup::gui
{

class AbstractAnyValueEditor;
class AnyValueItem;

/**
 * @brief The AnyValueEditorDialog class is a modal dialog containing one of AnyValue editors.
 */
class AnyValueEditorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AnyValueEditorDialog(std::unique_ptr<sup::gui::AbstractAnyValueEditor> editor,
                                QWidget* parent = nullptr);
  ~AnyValueEditorDialog() override;

  /**
   * @brief Sets initial value to the editor.
   */
  void SetInitialValue(const sup::gui::AnyValueItem* item);

  /**
   * @brief Returns result of the editing.
   *
   * It is supposed to be used after the dialog close.
   */
  std::unique_ptr<sup::gui::AnyValueItem> GetResult();

  void SetDescription(const QString& description);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  /**
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings();

  sup::gui::AbstractAnyValueEditor* m_anyvalue_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_DIALOG_H_
