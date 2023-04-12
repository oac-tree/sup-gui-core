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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_

#include <QWidget>

namespace sup::gui
{

//! A panel with a tree on the left side of AnyValueEditor.

class AnyValueEditorTreePanel : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorTreePanel(QWidget* parent = nullptr);
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TREEPANEL_H_
