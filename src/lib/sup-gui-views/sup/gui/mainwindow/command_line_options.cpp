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

#include "command_line_options.h"

#include <sup/gui/app/app_helper.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

namespace sup::gui
{

namespace
{

constexpr auto kInfoOption = "info";
constexpr auto kScaleOption = "scale";
constexpr auto kFontOption = "font";
constexpr auto kStyleOption = "style";
constexpr auto kFileOption = "file";
constexpr auto kWindowSizeOption = "size";

}  // namespace

void AddCommonOptions(QCommandLineParser& parser, const QString& file_option_description)
{
  parser.addHelpOption();
  parser.addVersionOption();

  const QCommandLineOption info_option(kInfoOption, "Show system environment information.");
  parser.addOption(info_option);

  const QCommandLineOption scale_option(
      kScaleOption,
      "Rely on system high-DPI scale via QT_ variables for 4K. If option is "
      "absent, will rely on internal scale mechanism (preferred).");
  parser.addOption(scale_option);

  QCommandLineOption font_option(kFontOption, "Main application font point size.");
  font_option.setValueName("pt");
  parser.addOption(font_option);

  QCommandLineOption style_option(kStyleOption,
                                  "Main GUI style, see --info for full list available.");
  style_option.setValueName("name");
  parser.addOption(style_option);

  const QCommandLineOption file_option({kFileOption, "f"}, file_option_description, "filename");
  parser.addOption(file_option);

  QCommandLineOption window_size_option(kWindowSizeOption, "Initial window size");
  window_size_option.setValueName("1024x768");
  parser.addOption(window_size_option);
}

Options GetCommonOptions(QCommandLineParser& parser)
{
  Options result;
  result.info = parser.isSet(kInfoOption);
  result.scale = parser.isSet(kScaleOption);

  if (parser.isSet(kFontOption))
  {
    result.system_font_psize = parser.value(kFontOption).toInt();
  }

  if (parser.isSet(kStyleOption))
  {
    result.style = parser.value(kStyleOption);
  }

  if (parser.isSet(kFileOption))
  {
    result.file_name = parser.value(kFileOption);
  }

  if (parser.isSet(kWindowSizeOption))
  {
    if (auto optional_size = ParseSizeString(parser.value(kWindowSizeOption));
        optional_size.has_value())
    {
      result.window_size = optional_size;
    }
    else
    {
      qInfo() << "Can't parse windows size option" << parser.value(kWindowSizeOption);
      parser.showHelp(1);
    }
  }

  return result;
}

Options ParseOptions(int argc, char** argv, const QString& file_option_description)
{
  // Parser requires an application to be created upfront.
  // Create an application solely for parser needs (fortunately it is cheap).
  const QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  AddCommonOptions(parser, file_option_description);
  parser.process(app);
  return GetCommonOptions(parser);
}

}  // namespace sup::gui
