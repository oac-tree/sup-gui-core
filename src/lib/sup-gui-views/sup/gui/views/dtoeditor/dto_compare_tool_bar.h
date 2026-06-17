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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOL_BAR_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOL_BAR_H_

#include <QToolBar>

class QCheckBox;
class QToolButton;
class QMenu;

namespace sup::gui
{

class DtoCompareToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit DtoCompareToolBar(QWidget* parent_widget = nullptr);

signals:
  void panelSyncAppearanceFlagChanged(bool value);

private:
  void InsertStrech();

  QCheckBox* m_sync_check_box{nullptr};
  QToolButton* m_dots_button{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOL_BAR_H_