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

#ifndef SUP_GUI_APP_MAIN_WINDOW_HELPER_H_
#define SUP_GUI_APP_MAIN_WINDOW_HELPER_H_

//! @file
//! Collection of helper methods for main windows.

#include <optional>

class QSize;
class QString;

namespace sup::gui
{

/**
 * @brief Opens message box with the question if we should reset application settings and restart
 *
 * @return Truth if reset and restart was requested, false otherwise.
 */
bool ShouldResetSettingsAndRestart();

/**
 * @brief Summons dialog to change system font.
 */
bool SummonChangeSystemFontDialog();

/**
 * @brief Returns size object from size string.
 *
 * "1024x768" -> QSize{1024, 768}
 * Will return empty optional, if conversion is not possible.
 */
std::optional<QSize> ParseSizeString(const QString& text);

}  // namespace sup::gui

#endif  // SUP_GUI_APP_MAIN_WINDOW_HELPER_H_
