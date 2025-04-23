/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#include "clipboard_helper.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>

namespace sup::gui
{

get_mime_data_func_t DefaultClipboardGetFunc()
{
  return []() { return QGuiApplication::clipboard()->mimeData(); };
}

set_mime_data_func_t DefaultClipboardSetFunc()
{
  return [](std::unique_ptr<QMimeData> data)
  { QGuiApplication::clipboard()->setMimeData(data.release()); };
}

}  // namespace oac_tree_gui
