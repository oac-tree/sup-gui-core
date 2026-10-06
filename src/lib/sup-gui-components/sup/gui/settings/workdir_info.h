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

#ifndef SUP_GUI_SETTINGS_WORKDIR_INFO_H_
#define SUP_GUI_SETTINGS_WORKDIR_INFO_H_

#include <string>

namespace sup::gui
{

/**
 * @brief The WorkdirInfo struct holds the last working directory of file dialogs.
 */
struct WorkdirInfo
{
  std::string path;  //!< the directory of the last selected file
};

template <typename Archive>
void Visit(Archive& archive, WorkdirInfo& info)
{
  archive("path", info.path);
}

}  // namespace sup::gui

#endif  // SUP_GUI_SETTINGS_WORKDIR_INFO_H_
