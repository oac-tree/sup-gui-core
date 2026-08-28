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

#ifndef SUP_GUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
#define SUP_GUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_

#include <QSize>
#include <QString>
#include <optional>

class QCommandLineParser;

namespace sup::gui
{

/**
 * @brief The Options struct contains the result of command line option parsing.
 *
 * It carries options common to all Operation Application GUIs.
 */
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

  //! the value provided via --file option; its interpretation is application specific
  QString file_name;

  //! initial window size
  std::optional<QSize> window_size;
};

//! Default description of --file option.
constexpr auto kDefaultFileOptionDescription = "Load AnyValue json from a given file name";

/**
 * @brief Populates the parser with options common to all Operation Application GUIs.
 *
 * @param parser The parser to populate.
 * @param file_option_description The description of --file option to show in the help message.
 */
void AddCommonOptions(QCommandLineParser& parser,
                      const QString& file_option_description = kDefaultFileOptionDescription);

/**
 * @brief Reads common options from an already processed parser.
 *
 * @param parser The parser previously populated via AddCommonOptions and already processed.
 * @return Structure with common options.
 */
Options GetCommonOptions(QCommandLineParser& parser);

/**
 * @brief Parses command line options and returns parsing result.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @param file_option_description The description of --file option to show in the help message.
 * @return Structure with common options.
 */
Options ParseOptions(int argc, char** argv,
                     const QString& file_option_description = kDefaultFileOptionDescription);

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_COMMAND_LINE_OPTIONS_H_
