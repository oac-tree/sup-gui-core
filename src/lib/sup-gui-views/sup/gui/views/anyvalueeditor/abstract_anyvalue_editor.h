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

  virtual void SetInitialValue(const sup::gui::AnyValueItem* item) = 0;

  virtual void SetDescription(const QString& description) {};

  virtual std::unique_ptr<sup::gui::AnyValueItem> GetResult() = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ABSTRACT_ANYVALUE_EDITOR_H_
