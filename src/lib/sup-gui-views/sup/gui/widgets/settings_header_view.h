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

#ifndef SUP_GUI_WIDGETS_SETTINGS_HEADER_VIEW_H_
#define SUP_GUI_WIDGETS_SETTINGS_HEADER_VIEW_H_

#include <sup/gui/settings/scoped_callback.h>

#include <mvvm/core/flags.h>

#include <QByteArray>
#include <QHeaderView>
#include <string>
#include <vector>

namespace sup::gui
{

class ISettingsManager;

/**
 * @brief The HeaderInfo struct holds the persistent state of a header.
 */
struct HeaderInfo
{
  QByteArray state;  //!< result of QHeaderView::saveState()
};

template <typename Archive>
void Visit(Archive& archive, HeaderInfo& info)
{
  archive("state", info.state);
}

/**
 * @brief The SettingsHeaderView class is a tree header storing column widths in application
 * settings.
 *
 * Column widths adjusted by the user are provided to the settings manager on Collect. Until the
 * user adjusts columns, the header provides the stored state, so it doesn't overwrite the state of
 * other headers of the same group.
 *
 * On the first show, columns get the stored widths, or widths following the stretch factors.
 * The context menu resets columns to the stretch factors.
 *
 * The settings manager must outlive the header.
 */
class SettingsHeaderView : public QHeaderView
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param group The settings group.
   * @param settings_manager The manager to store settings.
   * @param stretch_factors Stretch factors of columns. Equal widths if empty.
   * @param parent_widget The parent widget.
   */
  SettingsHeaderView(std::string group, ISettingsManager& settings_manager,
                     std::vector<int> stretch_factors = {}, QWidget* parent_widget = nullptr);

  /**
   * @brief Returns the state to store.
   *
   * This is the state adjusted by the user, or the stored state if the user didn't adjust columns.
   */
  HeaderInfo GetHeaderInfo() const;

  /**
   * @brief Sets columns to the user state, if any, or to the stretch factors.
   */
  void AdjustColumnsWidth();

  /**
   * @brief Forgets the user state and sets columns to the stretch factors.
   */
  void ResetColumnWidth();

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void showEvent(QShowEvent* event) override;

private:
  /**
   * @brief The InteractionState enum holds flags of user activity.
   */
  enum class InteractionState : std::uint8_t
  {
    kInteractiveMode,  //!< the user is resizing columns
    kWasShown,         //!< the header was shown at least once
    kWasAdjusted,      //!< the user adjusted columns at least once
    size
  };

  //! Shows the context menu to reset columns.
  void OnContextMenuRequest(const QPoint& point);

  //! Saves the state on interactive resize. Ignores programmatic resize.
  void OnSectionResize(int index, int prev_size, int new_size);

  std::string m_group;
  ISettingsManager& m_settings_manager;
  std::vector<int> m_stretch_factors;
  QByteArray m_favorite_state;  //!< the user state, or the stored state
  mvvm::Flags<InteractionState> m_interaction_state;
  ScopedCallback m_provider_connection;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_SETTINGS_HEADER_VIEW_H_
