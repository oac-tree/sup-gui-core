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

#ifndef SUP_GUI_APP_APP_HELPER_H_
#define SUP_GUI_APP_APP_HELPER_H_

//! @file
//! Helper functions for application.

#include <optional>

class QString;
class QSize;

namespace sup::gui
{

/**
 * @brief Returns true if application is running in CODAC environment.
 */
bool IsOnCodac();

/**
 * @brief Returns size object from size string.
 *
 * "1024x768" -> QSize{1024, 768}
 * Will return empty optional, if conversion is not possible.
 */
std::optional<QSize> ParseSizeString(const QString& text);

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_HELPER_H_
