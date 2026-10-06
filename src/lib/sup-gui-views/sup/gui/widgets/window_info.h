/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#ifndef SUP_GUI_WIDGETS_WINDOW_INFO_H_
#define SUP_GUI_WIDGETS_WINDOW_INFO_H_

#include <QByteArray>

namespace sup::gui
{

/**
 * @brief The WindowInfo struct holds the persistent geometry of a window.
 */
struct WindowInfo
{
  QByteArray geometry;  //!< result of QWidget::saveGeometry()
};

template <typename Archive>
void Visit(Archive& archive, WindowInfo& info)
{
  archive("geometry", info.geometry);
}

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_WINDOW_INFO_H_
