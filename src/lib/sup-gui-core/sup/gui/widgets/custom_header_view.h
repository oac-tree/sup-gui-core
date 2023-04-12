/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
 * @brief The CustomHeaderView class provides a header for QTreeView that remembers column size as
 * they have been interactively adjusted by the user.
 */

class CustomHeaderView : public QHeaderView
{
  Q_OBJECT

public:
  explicit CustomHeaderView(QWidget *parent);

  void SetAsFavoriteState(QByteArray state);

  void RestoreFavoriteState();

  bool HasFavoriteState() const;

  QByteArray GetFavoriteState() const;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  void OnSectionResize(int index, int prev_size, int new_size);

  bool m_is_in_interactive_mode{false};
  QByteArray m_favorite_state;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_CUSTOM_HEADER_VIEW_H_
