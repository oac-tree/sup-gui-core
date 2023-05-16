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

#include "code_editor_sidebar.h"

#include "code_editor.h"

namespace sup::gui
{
CodeEditorSidebar::CodeEditorSidebar(CodeEditor *editor) : QWidget(editor), m_code_editor(editor) {}

QSize CodeEditorSidebar::sizeHint() const
{
//  return QSize(m_code_editor->sidebarWidth(), 0);
    return {};
}

void CodeEditorSidebar::paintEvent(QPaintEvent *event)
{
//  m_code_editor->sidebarPaintEvent(event);
}

void CodeEditorSidebar::mouseReleaseEvent(QMouseEvent *event)
{
//  if (event->pos().x() >= width() - m_code_editor->fontMetrics().lineSpacing()) {
//      auto block = m_code_editor->blockAtPosition(event->pos().y());
//      if (!block.isValid() || !m_code_editor->isFoldable(block)) {
//          return;
//      }
//      m_code_editor->toggleFold(block);
//  }
//  QWidget::mouseReleaseEvent(event);

}

}  // namespace sup::gui
