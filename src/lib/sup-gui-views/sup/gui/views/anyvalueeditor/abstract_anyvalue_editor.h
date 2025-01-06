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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ABSTRACT_ANYVALUE_EDITOR_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ABSTRACT_ANYVALUE_EDITOR_H_

#include <QWidget>

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief The AbstractAnyValueEditor class it's a base class for all widgets intended for AnyValue
 * editing.
 */
class AbstractAnyValueEditor : public QWidget
{
  Q_OBJECT

public:
  explicit AbstractAnyValueEditor(QWidget* parent) : QWidget(parent) {}

  /**
   * @brief Sets initial value.
   *
   * The given value will be cloned inside the editor's model and used as a starting point for
   * editing.
   */
  virtual void SetInitialValue(const sup::gui::AnyValueItem* item) = 0;

  /**
   * @brief Sets the description of the editor.
   */
  virtual void SetDescription(const QString& description) {};

  /**
   * @brief Returns the result of editing.
   */
  virtual std::unique_ptr<sup::gui::AnyValueItem> GetResult() = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ABSTRACT_ANYVALUE_EDITOR_H_
