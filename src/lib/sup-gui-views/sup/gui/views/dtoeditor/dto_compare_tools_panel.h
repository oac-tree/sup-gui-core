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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_

#include <QWidget>

namespace sup::gui
{

class DtoCompareToolsPanel : public QWidget
{
  Q_OBJECT

public:
  explicit DtoCompareToolsPanel(QWidget* parent_widget = nullptr);
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_
