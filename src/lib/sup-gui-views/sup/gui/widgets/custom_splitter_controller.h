/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_
#define SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_

#include <sup/gui/widgets/settings_callbacks.h>

class QSplitter;
#include <QObject>

namespace sup::gui
{

/**
 * @brief The CustomSplitterController class assists QSplitter in reading/writing the preferred
 * state from/to application persistent settings.
 *
 * The state includes the usual splitter panel size and collapsed/expanded status of panels,
 * as well as the hidden status of widgets embedded in panels.
 *
 * Possible usage scenario:
 * The user constructs a splitter and adds necessary widgets to it. Then, the controller reads
 * splitter settings from persistent storage and sets it to a splitter. At the end of work, the
 * controller saves persistent settings.
 *
 * The class has a listening mechanism to update children's visibility flag and to properly handle
 * show/hide panel user request. This assures that even in the destruction phase all last known
 * children's visibility status will be correctly recorded.
 *
 * A load of settings is done on user request since only the user knows when the splitter is fully
 * populated with the content. Also, it is required to call the StartChildrenListening() method to
 * handle show/hide events. The save of the settings can be done at any time, even on application
 * destruction.
 */
class CustomSplitterController : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor to construct controller.
   *
   * The setting's group name can be in a form "WidgetName/splitter" so all settings related to
   * WidgetName are grouped in the same place of QSetting file.
   *
   * @param settings_group_name The name of the setting group in persistenf storage.
   * @param splitter The splitter under control.
   */
  explicit CustomSplitterController(const QString& settings_group_name, QSplitter* splitter);

  /**
   * @brief Read settings from storage using function provided.
   */
  void ReadSettings(const read_variant_func_t& read_func);

  /**
   * @brief Write settings to persistent storage using function provided.
   *
   * This method can be safely called at any moment, even on the main window destruction. All last
   * known visibility flags of widgets populating a splitter will be correctly preserved.
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

  /**
   * @brief Will listen children for HideToParent and ShowToParent events to update children
   * visibility cache.
   *
   * This method shall be called after all children have been added to a splitter.
   */
  void StartChildrenListening();

  /**
   * @brief Update cache for children visibility flags.
   */
  void UpdateChildrenVisibilityCache();

protected:
  /**
   * @brief Updates children visibility flag.
   */
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  QSplitter* m_splitter{nullptr};
  QString m_settings_group_name;           //!< group name in QSettings file
  QList<int> m_children_visibility_flags;  //!< children widget visibility flags
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_SPLITTER_CONTROLLER_H_
