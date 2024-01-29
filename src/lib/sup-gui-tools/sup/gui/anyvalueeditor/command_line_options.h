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

#ifndef SUP_GUI_ANYVALUEEDITOR_COMMAND_LINE_OPTIONS_H_
#define SUP_GUI_ANYVALUEEDITOR_COMMAND_LINE_OPTIONS_H_

//! Command line options.

#include <QString>

namespace sup::gui
{

struct Options
{
  //! rely on system scale via QT_ variables, if true
  bool scale = false;

  //! print system environment information
  bool info = false;

  //! appplication system font point size
  int system_font_psize = -1;

  //! main appplication GUI style
  QString style;
};

//! Parse command line options.
Options ParseOptions(int argc, char** argv);

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_COMMAND_LINE_OPTIONS_H_
