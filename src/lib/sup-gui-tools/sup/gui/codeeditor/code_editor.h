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

#include <QPlainTextEdit>
#include <memory>

namespace sup::gui
{

/**
 * @brief The CodeEditor class is a light-weighted code editor with syntax highlighting and code
 * folding.
 *
 * @note Rely on KDE syntax highlighter https://github.com/KDE/syntax-highlighting
 */

class CodeEditor : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit CodeEditor(QWidget *parent = nullptr);
  ~CodeEditor() override;

  /**
   * @brief Set editor content. Previous content will be cleared up.
   */
  void SetText(const QString &text, const QString &definition_name = {});

  /**
   * @brief Set language definition for given name (JSON, XML)
   */
  void SetDefinition(const QString &definition_name);

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  friend class CodeEditorSidebar;

  int sidebarWidth() const;
  void sidebarPaintEvent(QPaintEvent *event);
  void updateSidebarGeometry();
  void updateSidebarArea(const QRect &rect, int dy);

  QTextBlock blockAtPosition(int y) const;
  bool isFoldable(const QTextBlock &block) const;
  bool isFolded(const QTextBlock &block) const;
  void toggleFold(const QTextBlock &block);

  struct CodeEditorImpl;
  std::unique_ptr<CodeEditorImpl> p_impl;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CODEEDITOR_CODEEDITOR_H_
