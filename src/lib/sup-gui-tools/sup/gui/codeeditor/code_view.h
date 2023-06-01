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

#ifndef SUP_GUI_CODEEDITOR_CODE_VIEW_H_
#define SUP_GUI_CODEEDITOR_CODE_VIEW_H_

#include <QWidget>

namespace sup::gui
{

class CodeEditor;

/**
 * @brief Simple viewer of XML/JSON content.
 *
 * @details It is intended for dynamic updates of text content while preserving the position
 * of the scroll area. It allows to monitor the XML/JSON representation of the model in the
 * course of model update.
 */

class CodeView : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Supported language definitions.
   */
  enum LanguageDefinition
  {
    kXML,
    kJSON
  };

  explicit CodeView(LanguageDefinition language = kXML, QWidget* parent = nullptr);
  ~CodeView() override;

  void SetFile(const QString& file_name);

  void SetContent(const QString& content);

  void ClearText();

  void OnExportToFileRequest();

private:
  struct ScrollBarPosition
  {
    int vertical{0};
    int horizontal{0};
  };

  void ReadSettings();
  void WriteSettings();
  void SaveScrollBarPosition();
  void RestoreScrollBarPosition();

  CodeEditor* m_text_edit{nullptr};
  QString m_current_workdir;
  ScrollBarPosition m_cached_scrollbar_pos;
  LanguageDefinition m_language;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CODEEDITOR_CODE_VIEW_H_
