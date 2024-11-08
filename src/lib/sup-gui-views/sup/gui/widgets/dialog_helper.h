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

#ifndef SUP_GUI_WIDGETS_DIALOG_HELPER_H_
#define SUP_GUI_WIDGETS_DIALOG_HELPER_H_

#include <memory>
#include <QString>

class QVBoxLayout;
class QDialog;

namespace sup::gui
{

/**
 * @brief Creates layout with OK/Cancel buttons.
 */
std::unique_ptr<QVBoxLayout> CreateButtonLayout(QDialog* dialog, const QString& ok_text = "OK",
                                                const QString& cancel_text = "Cancel");


}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_DIALOG_HELPER_H_
