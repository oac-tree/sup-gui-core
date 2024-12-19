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

#ifndef SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_
#define SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_

#include <QSplitter>

namespace sup::gui
{

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
   * For any non-empty setting name, the class will write its favorite state on destruction using
   * QSettings machinery. It will be used during the next splitter construction. The setting's name
   * can be "WidgetName/splitter" so the splitter state would be grouped with other settings of
   * certain widget in QSetting file.
   *
   * @param setting_name The name of this splitter related settings in the setting file.
   * @param parent_widget The parent widget of this splitter.
   */
  explicit CustomSplitter(const QString& setting_name, QWidget* parent_widget = nullptr);
  ~CustomSplitter() override;

  /**
   * @brief Reads splitter state from disk.
   *
   * This method shall be called after all widgets have been already added.
   */
  void ReadSettings();

protected:
  void childEvent(QChildEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;
  void showEvent(QShowEvent* event) override;
  void hideEvent(QHideEvent* event) override;

private:
  /**
   * @brief Writes splitter state.
   *
   * The method called automatically on widget
   */
  void WriteSettings();

  QString m_setting_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_SPLITTER_H_
