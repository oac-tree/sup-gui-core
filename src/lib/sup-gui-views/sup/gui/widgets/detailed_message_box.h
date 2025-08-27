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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_WIDGETS_DETAILED_MESSAGE_BOX_H_
#define SUP_GUI_WIDGETS_DETAILED_MESSAGE_BOX_H_

#include <sup/gui/core/message_event.h>

#include <QDialog>
#include <memory>

class QPlainTextEdit;
class QLabel;
class QLayout;

namespace sup::gui
{

/**
 * @brief The DetailedMessageBox class is a dialog similar to QMessageBox with a place for large
 * text output.
 *
 * It contains a non editable text edit field with scroll bars, and a single OK button.
 */
class DetailedMessageBox : public QDialog
{
  Q_OBJECT
public:
  /**
   * @brief Creates a message box and sets its content according to the given message event.
   */
  explicit DetailedMessageBox(const MessageEvent& message, QWidget* parent_widget = nullptr);

  /**
   * @brief Creates a message box and sets its content according to the given message event.
   *
   * Allow to define a setting key to remember the dialog size and position.
   *
   * @param setting_group A name of the group to store/restore dialog settings.
   * @param message A message event to display.
   * @param parent_widget An optional parent widget.
   */
  DetailedMessageBox(const QString& setting_group, const MessageEvent& message,
                     QWidget* parent_widget = nullptr);

  QPlainTextEdit* GetTextEdit() const;

private:
  std::unique_ptr<QLayout> CreateIconAndTextLayout();
  std::unique_ptr<QLayout> CreateInformativeLayout(const std::string& informative);
  static std::unique_ptr<QLayout> CreateIconLayout();

  /**
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings();

  QString m_setting_group;
  QLabel* m_text_label{nullptr};
  QLabel* m_informative_label{nullptr};
  QPlainTextEdit* m_detailed_text_edit{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_DETAILED_MESSAGE_BOX_H_
