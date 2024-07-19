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

#ifndef SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_
#define SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{

class CollapsibleToolBar;

//! Wraps user widget and makes it collapsible.
//! Intended for usage with a QSplitter. The user widget is inserted into the splitter
//! with a special toolbar, containing widget actions and a collapse/expand button.

class CollapsibleWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CollapsibleWidget(QWidget* context, QWidget* parent = nullptr);
  ~CollapsibleWidget() override;

  void AddToSplitter(QSplitter* splitter);

  CollapsibleToolBar* GetToolBar();

private:
  CollapsibleToolBar* m_tool_bar{nullptr};  // ToolBar intended to go to splitter separately
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_COLLAPSIBLE_WIDGET_H_
