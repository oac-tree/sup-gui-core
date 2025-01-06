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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_
#define SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_

#include <QSplitter>
#include <memory>

namespace sup::gui
{

class CustomSplitterController;

/**
 * @brief The CustomSplitter class is a normal splitter that remembers the size of panels and hidden
 * status of widgets.
 *
 * It can save its state on disk in persistent settings and can restore that state on the next
 * construction.
 *
 * The persistent state consists of two parts:
 * 1) The normal state of the splitter, i.e. panel sizes and collapsed status, as provided by
 * QSplitter::saveState method.
 * 2) Additional hidden status for widgets.
 */
class CustomSplitter : public QSplitter
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor
   *
   * @param setting_name The name of a setting key in QSettings file.
   * @param parent_widget The parent widget of this splitter.
   *
   * @note The setting's name can be "WidgetName/splitter" so the splitter state would be grouped
   * with other settings of certain widget in QSetting file.
   */
  explicit CustomSplitter(const QString& setting_name, QWidget* parent_widget = nullptr);
  ~CustomSplitter() override;

  /**
   * @brief Reads splitter state from disk.
   *
   * This method shall be called after all widgets have been already added.
   */
  void ReadSettings();

  /**
   * @brief Writes splitter state.
   *
   * This method can be safely called at any moment, even on the main window destruction. All last
   * known visibility flags of widgets populating a splitter will be correctly preserved.
   */
  void WriteSettings();

private:
  std::unique_ptr<CustomSplitterController> m_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_
