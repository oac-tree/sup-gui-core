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

#ifndef SUP_GUI_VIEWS_CODEEDITOR_CODE_EDITOR_SIDEBAR_H_
#define SUP_GUI_VIEWS_CODEEDITOR_CODE_EDITOR_SIDEBAR_H_

#include <QWidget>

namespace sup::gui
{

class CodeEditor;

//! A side bar with line numbers for code editor.

class CodeEditorSidebar : public QWidget
{
  Q_OBJECT

public:
  explicit CodeEditorSidebar(CodeEditor *editor);

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  CodeEditor *m_code_editor;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_CODEEDITOR_CODE_EDITOR_SIDEBAR_H_
