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

#include "code_editor_sidebar.h"

#include <definition.h>
#include <repository.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include <QApplication>
#include <QDebug>

namespace sup::gui
{

/**
 * @brief The CodeEditorImpl class contains implementation details of CodeEditor class.
 * Invented to hide syntax-highlighter internals from users of CodeEditor class.
 *
 */
struct CodeEditor::CodeEditorImpl
{
  KSyntaxHighlighting::Repository m_repository;
  KSyntaxHighlighting::SyntaxHighlighter* m_highlighter{nullptr};
  QPlainTextEdit* m_self{nullptr};

  CodeEditorImpl(QPlainTextEdit* self) : m_self(self)

  {
    m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(m_self->document());
    qDebug() << m_highlighter;
  }

  /**
   * @brief Returns true if currently we have dark desktop theme.
   */
  bool IsDarkMode() { return m_self->palette().color(QPalette::Base).lightness() < 128; }

  /**
   * @brief Sets editor main highlighting theme.
   */
  void SetTheme(const KSyntaxHighlighting::Theme& theme)
  {
    auto pal = qApp->palette();
    if (theme.isValid())
    {
      pal.setColor(QPalette::Base, theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
      pal.setColor(QPalette::Highlight,
                   theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
    }
    m_self->setPalette(pal);

    m_highlighter->setTheme(theme);
    m_highlighter->rehighlight();
    //    highlightCurrentLine();
  }

  /**
   * @brief SetDefaultTheme
   */
  void SetDefaultTheme()
  {
    SetTheme(IsDarkMode() ? m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                          : m_repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
  }
};

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent), p_impl(std::make_unique<CodeEditorImpl>(this))
{
  setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

CodeEditor::~CodeEditor() = default;

void CodeEditor::SetText(const QString& text)
{
  setPlainText(text);

  p_impl->SetDefaultTheme();

  const auto def = p_impl->m_repository.definitionForFileName("aaa.json");
  p_impl->m_highlighter->setDefinition(def);
}

}  // namespace sup::gui
