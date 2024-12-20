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

#include <sup/gui/widgets/settings_callbacks.h>

class QSplitter;

namespace sup::gui
{

/**
 * @brief The CustomSplitterController class assists QSplitter in reading/writing the preferred
 * state from/to application persistent settings.
 *
 * The state includes the usual splitter panel size and collapsed/expanded status of panels,
 * as well as the hidden status of widgets embedded in panels. It is used in the following scenario.
 * The user constructs a splitter and adds necessary widgets to it. Then, the controller reads
 * splitter settings from persistent storage and sets it to a splitter. At the end of work, the
 * controller saves persistent settings.
 *
 * Settings save and load is done on user request since only the user knows when the splitter is
 * fully populated with the content and when the application is about to finish its work. We
 * deliberately do not rely on showEvent and hideEvent in that because of their spurious nature.
 */
class CustomSplitterController
{
public:
  /**
   * @brief Main c-tor to construct controller.
   *
   * The setting's group name can be in a form "WidgetName/splitter" so all settings related to
   * WidgetName are grouped in the same place of QSetting file.
   *
   * @param setting_group_name The name of the setting group in persistenf storage.
   * @param splitter The splitter under control.
   */
  explicit CustomSplitterController(const QString& setting_group_name, QSplitter* splitter);

  /**
   * @brief Read settings from storage using function provided.
   */
  void ReadSettings(const read_variant_func_t& read_func);

  /**
   * @brief Write settings to persistent storage using function provided.
   */
  void WriteSettings(const write_variant_func_t& write_func);

  /**
   * @brief Get setting name used to store the main state of the splitter.
   */
  QString GetMainStateKey();

  /**
   * @brief Get setting name used to store children visibility information.
   */
  QString GetChildrenStateKey();

private:
  QSplitter* m_splitter{nullptr};
  QString m_settings_group_name;  //!< group name in QSettings file
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_
