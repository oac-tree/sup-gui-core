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

#include "code_editor.h"

namespace sup::gui
{

struct CodeEditor::CodeEditorImpl
{
};

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent), p_impl(std::make_unique<CodeEditorImpl>())
{
}

CodeEditor::~CodeEditor() = default;

}  // namespace sup::gui
