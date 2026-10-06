/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWS_CODEEDITOR_CODE_VIEW_H_
#define SUP_GUI_VIEWS_CODEEDITOR_CODE_VIEW_H_

#include <QWidget>

namespace sup::gui
{

class CodeEditor;
class ISettingsManager;

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

  /**
   * @brief Main c-tor.
   *
   * The export directory is stored in application settings. The settings manager must outlive the
   * view.
   */
  CodeView(LanguageDefinition language, ISettingsManager& settings_manager,
           QWidget* parent_widget = nullptr);
  ~CodeView() override;

  CodeView(const CodeView&) = delete;
  CodeView& operator=(const CodeView&) = delete;
  CodeView(CodeView&&) = delete;
  CodeView& operator=(CodeView&&) = delete;

  void SetFile(const QString& file_name);

  void SetContent(const QString& content);

  QString GetContent() const;

  void ClearText();

  void OnExportToFileRequest();

  CodeEditor* GetCodeEditor();

private:
  struct ScrollBarPosition
  {
    int vertical{0};
    int horizontal{0};
  };

  void SaveScrollBarPosition();
  void RestoreScrollBarPosition();

  CodeEditor* m_text_edit{nullptr};
  ScrollBarPosition m_cached_scrollbar_pos;
  LanguageDefinition m_language;
  ISettingsManager& m_settings_manager;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_CODEEDITOR_CODE_VIEW_H_
