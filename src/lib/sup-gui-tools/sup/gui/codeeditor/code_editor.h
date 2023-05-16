/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_CODEEDITOR_CODE_EDITOR_H_
#define SUP_GUI_CODEEDITOR_CODE_EDITOR_H_

#include <QTextEdit>
//#include <QPlainTextEdit>
#include <memory>

namespace sup::gui
{

//! A light-weighted code editor with syntax highlight and code folding.

class CodeEditor : public QTextEdit
{
  Q_OBJECT

public:
  explicit CodeEditor(QWidget* parent = nullptr);
  ~CodeEditor() override;

private:
  struct CodeEditorImpl;
  std::unique_ptr<CodeEditorImpl> p_impl;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CODEEDITOR_CODEEDITOR_H_
