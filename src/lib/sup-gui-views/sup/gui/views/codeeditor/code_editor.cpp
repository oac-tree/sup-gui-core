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

#include "code_editor.h"

#include "code_editor_sidebar.h"

#include <definition.h>
#include <repository.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include <QApplication>
#include <QDebug>
#include <QPainter>

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
  CodeEditorSidebar* m_sideBar;
  CodeEditor* m_self{nullptr};

  explicit CodeEditorImpl(CodeEditor* self)
      : m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(self->document()))
      , m_sideBar(new CodeEditorSidebar(self))
      , m_self(self)

  {
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
    highlightCurrentLine();
  }

  /**
   * @brief SetDefaultTheme
   */
  void SetDefaultTheme()
  {
    SetTheme(IsDarkMode() ? m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                          : m_repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
  }

  void highlightCurrentLine()
  {
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(
        QColor(m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CurrentLine)));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = m_self->textCursor();
    selection.cursor.clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    m_self->setExtraSelections(extraSelections);
  }
};

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent), p_impl(std::make_unique<CodeEditorImpl>(this))
{
  setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

  setLineWrapMode(QPlainTextEdit::NoWrap);
  p_impl->SetDefaultTheme();

  connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateSidebarGeometry);
  connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateSidebarArea);
  connect(this, &QPlainTextEdit::cursorPositionChanged, this,
          [this]() { p_impl->highlightCurrentLine(); });

  updateSidebarGeometry();
  p_impl->highlightCurrentLine();
}

CodeEditor::~CodeEditor() = default;

void CodeEditor::SetText(const QString& text, const QString& definition_name)
{
  clear();

  if (!definition_name.isEmpty())
  {
    SetDefinition(definition_name);
  }
  setPlainText(text);
}

void CodeEditor::SetDefinition(const QString& definition_name)
{
  const auto def = p_impl->m_repository.definitionForName(definition_name);
  p_impl->m_highlighter->setDefinition(def);
}

void CodeEditor::resizeEvent(QResizeEvent* event)
{
  QPlainTextEdit::resizeEvent(event);
  updateSidebarGeometry();
}

int CodeEditor::sidebarWidth() const
{
  int digits = 1;
  auto count = blockCount();
  while (count >= 10)
  {
    ++digits;
    count /= 10;
  }
  return 4 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits
         + fontMetrics().lineSpacing();
}

void CodeEditor::sidebarPaintEvent(QPaintEvent* event)
{
  QPainter painter(p_impl->m_sideBar);
  painter.fillRect(event->rect(), p_impl->m_highlighter->theme().editorColor(
                                      KSyntaxHighlighting::Theme::IconBorder));

  auto block = firstVisibleBlock();
  auto blockNumber = block.blockNumber();
  int top = blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + blockBoundingRect(block).height();
  const int currentBlockNumber = textCursor().blockNumber();

  const auto foldingMarkerSize = fontMetrics().lineSpacing();

  while (block.isValid() && top <= event->rect().bottom())
  {
    if (block.isVisible() && bottom >= event->rect().top())
    {
      const auto number = QString::number(blockNumber + 1);
      painter.setPen(p_impl->m_highlighter->theme().editorColor(
          (blockNumber == currentBlockNumber) ? KSyntaxHighlighting::Theme::CurrentLineNumber
                                              : KSyntaxHighlighting::Theme::LineNumbers));
      painter.drawText(0, top, p_impl->m_sideBar->width() - 2 - foldingMarkerSize,
                       fontMetrics().height(), Qt::AlignRight, number);
    }

    // folding marker
    if (block.isVisible() && isFoldable(block))
    {
      QPolygonF polygon;
      if (isFolded(block))
      {
        polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.25);
        polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.75);
        polygon << QPointF(foldingMarkerSize * 0.8, foldingMarkerSize * 0.5);
      }
      else
      {
        polygon << QPointF(foldingMarkerSize * 0.25, foldingMarkerSize * 0.4);
        polygon << QPointF(foldingMarkerSize * 0.75, foldingMarkerSize * 0.4);
        polygon << QPointF(foldingMarkerSize * 0.5, foldingMarkerSize * 0.8);
      }
      painter.save();
      painter.setRenderHint(QPainter::Antialiasing);
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor(
          p_impl->m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CodeFolding)));
      painter.translate(p_impl->m_sideBar->width() - foldingMarkerSize, top);
      painter.drawPolygon(polygon);
      painter.restore();
    }

    block = block.next();
    top = bottom;
    bottom = top + blockBoundingRect(block).height();
    ++blockNumber;
  }
}

void CodeEditor::updateSidebarGeometry()
{
  setViewportMargins(sidebarWidth(), 0, 0, 0);
  const auto r = contentsRect();
  p_impl->m_sideBar->setGeometry(QRect(r.left(), r.top(), sidebarWidth(), r.height()));
}

void CodeEditor::updateSidebarArea(const QRect& rect, int dy)
{
  if (dy)
  {
    p_impl->m_sideBar->scroll(0, dy);
  }
  else
  {
    p_impl->m_sideBar->update(0, rect.y(), p_impl->m_sideBar->width(), rect.height());
  }
}

QTextBlock CodeEditor::blockAtPosition(int y) const
{
  auto block = firstVisibleBlock();
  if (!block.isValid())
  {
    return QTextBlock();
  }

  int top = blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + blockBoundingRect(block).height();
  do
  {
    if (top <= y && y <= bottom)
    {
      return block;
    }
    block = block.next();
    top = bottom;
    bottom = top + blockBoundingRect(block).height();
  } while (block.isValid());
  return QTextBlock();
}

bool CodeEditor::isFoldable(const QTextBlock& block) const
{
  return p_impl->m_highlighter->startsFoldingRegion(block);
}

bool CodeEditor::isFolded(const QTextBlock& block) const
{
  if (!block.isValid())
  {
    return false;
  }
  const auto nextBlock = block.next();
  if (!nextBlock.isValid())
  {
    return false;
  }
  return !nextBlock.isVisible();
}

void CodeEditor::toggleFold(const QTextBlock& startBlock)
{
  // we also want to fold the last line of the region, therefore the ".next()"
  const auto endBlock = p_impl->m_highlighter->findFoldingRegionEnd(startBlock).next();

  if (isFolded(startBlock))
  {
    // unfold
    auto block = startBlock.next();
    while (block.isValid() && !block.isVisible())
    {
      block.setVisible(true);
      block.setLineCount(block.layout()->lineCount());
      block = block.next();
    }
  }
  else
  {
    // fold
    auto block = startBlock.next();
    while (block.isValid() && block != endBlock)
    {
      block.setVisible(false);
      block.setLineCount(0);
      block = block.next();
    }
  }

  // redraw document
  document()->markContentsDirty(startBlock.position(),
                                endBlock.position() - startBlock.position() + 1);

  // update scrollbars
  Q_EMIT document() -> documentLayout()->documentSizeChanged(
                        document()->documentLayout()->documentSize());
}

}  // namespace sup::gui
