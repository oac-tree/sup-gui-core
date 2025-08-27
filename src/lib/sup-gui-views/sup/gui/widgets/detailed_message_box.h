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

#include <QDialog>
#include <sup/gui/core/message_event.h>
#include <memory>

class QTextEdit;
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
  explicit DetailedMessageBox(const MessageEvent& message, QWidget* parent_widget = nullptr);

  QTextEdit* GetTextEdit() const;

private:
  std::unique_ptr<QLayout> CreateIconAndTextLayout();
  std::unique_ptr<QLayout> CreateInformativeLayout(const std::string& informative);
  static std::unique_ptr<QLayout> CreateIconLayout();

  QLabel* m_text_label{nullptr};
  QLabel* m_informative_label{nullptr};
  QTextEdit* m_detailed_text_edit{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_DETAILED_MESSAGE_BOX_H_
