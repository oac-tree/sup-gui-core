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

#ifndef SUP_GUI_WIDGETS_CUSTOM_HEADER_VIEW_H_
#define SUP_GUI_WIDGETS_CUSTOM_HEADER_VIEW_H_

#include <QByteArray>
#include <QHeaderView>

namespace sup::gui
{

/**
 * @brief The CustomHeaderView class provides a header for QTreeView that remembers column size
 * following interactive adjustment by the user.
 *
 * It also can save its state on disk in persistent settings and can restore to that state on the
 * next construction. It has a context menu: on the right mouse button, one can reset the column
 * width to initial settings.
 */
class CustomHeaderView : public QHeaderView
{
  Q_OBJECT

public:
  /**
   * @brief The base c-tor.
   *
   * The header constructed in this way knows nothing about optimal tree column width, and doesn't
   * try to restore own state from persistent settings on disk.
   */
  explicit CustomHeaderView(QWidget *parent);

  ~CustomHeaderView();

  /**
   * @brief The constructor that invokes persistent setting machinery.
   *
   * For any non-empty setting name, the class will write its favorite state on destruction using
   * QSettings machinery. It will be used during the next header construction. The setting's name
   * can be "WidgetName/header_state" so the header state would be grouped with other settings of
   * certain widgets in QSetting file.
   */
  CustomHeaderView(const QString &setting_name, QWidget *parent);

  /**
   * @brief Sets given state as a favorite state.
   *
   * It will be restored later on RestoreFavoriteState call.
   */
  void SetAsFavoriteState(QByteArray state);

  /**
   * @brief Restore state (width of tree's columns) stored earlier as a favorite state.
   */
  void RestoreFavoriteState();

  /**
   * @brief Checks if the header has a favorite state (width of columns).
   *
   * Favorite state appears when the user starts to resize tree columns manually.
   */
  bool HasFavoriteState() const;

  /**
   * @brief Returns favorite state.
   *
   * Favorite state appears when the user starts to resize tree columns manually.
   */
  QByteArray GetFavoriteState() const;

  /**
   * @brief Adjusts column width.
   *
   * Will use the favorite state, if available. Otherwise will use optimum column with.
   */
  void AdjustColumnsWidth();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  /**
   * @brief Reads header state from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes header state.
   */
  void WriteSettings();

  /**
   * @brief Save size of columns in a header on any interactive resize activity.
   *
   * Will ignore programmatic column resize (i.e. due to resize of the parent widget).
   */
  void OnSectionResize(int index, int prev_size, int new_size);

  bool m_is_in_interactive_mode{false};
  QByteArray m_favorite_state;
  QString m_setting_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_HEADER_VIEW_H_
