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

#ifndef SUP_GUI_PROJECT_RECENT_PROJECT_SETTINGS_H_
#define SUP_GUI_PROJECT_RECENT_PROJECT_SETTINGS_H_

#include <sup/gui/project/recent_project_path.h>

namespace sup::gui
{

/**
 * @brief Collection of settings for persistence save the last directory selected by the user, and
 * list of recent projects.
 */
class RecentProjectSettings : public RecentProjectPath
{
public:
  explicit RecentProjectSettings(const QString& group_name = "RecentProjectSettings");
  ~RecentProjectSettings() override;

private:
  /**
   * @brief Writes settings to disk.
   */
  void WriteSettings();

  /**
   * @brief Reads settings from disk.
   */
  void ReadSettings();

  QString m_group_name;  //!< the name of the group for persistent settings
};

}  // namespace sup::gui

#endif  // SUP_GUI_PROJECT_RECENT_PROJECT_SETTINGS_H_
