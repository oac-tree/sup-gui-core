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

#ifndef SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_
#define SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_

class QSplitter;

namespace sup::gui
{

/**
 * @brief The CustomSplitterController class assists QSplitter in saving and loading the preferred
 * state.
 *
 * The state includes the usual splitter panel size and collapsed/expanded status of panels,
 * as well as the hidden status of widgets embedded in panels. It is used in the following scenario.
 * The user constructs a splitter and adds necessary widgets to it. Then, the controller reads
 * splitter settings from persistent storage and sets it to a splitter. At the end of work, the
 * controller saves persistent settings.
 *
 * Settings save and load is done on user request since only the user knows when the splitter is
 * fully populated with the content and when the work has finished. We deliberately do not rely on
 * showEvent and hideEvent in that because of their spurious nature.
 */
class CustomSplitterController
{
public:
  explicit CustomSplitterController(QSplitter* splitter);

  void ReadSettings();

  void WriteSettings();

private:
  QSplitter* m_splitter{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_
