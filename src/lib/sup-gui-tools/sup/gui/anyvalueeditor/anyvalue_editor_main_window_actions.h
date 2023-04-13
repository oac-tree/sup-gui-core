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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_

#include <QObject>

class QMainWindow;

namespace anyvalueeditor
{

class AnyValueEditorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorMainWindowActions(QMainWindow* parent = nullptr);
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
