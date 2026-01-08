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

#ifndef SUP_GUI_WIDGETS_FLOW_LAYOUT_H_
#define SUP_GUI_WIDGETS_FLOW_LAYOUT_H_

#include <QLayout>
#include <QRect>
#include <QStyle>

namespace sup::gui
{

/**
 * @brief The FlowLayout class implements a layout that handles different window sizes.
 *
 * The widget placement changes depending on the width of the application window.
 * Borrowed from Qt example section https://doc.qt.io/qt-6/qtwidgets-layouts-flowlayout-example.html
 */
class FlowLayout : public QLayout
{
public:
  explicit FlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
  explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
  ~FlowLayout() override;

  FlowLayout(const FlowLayout&) = delete;
  FlowLayout& operator=(const FlowLayout&) = delete;
  FlowLayout(FlowLayout&&) = delete;
  FlowLayout& operator=(FlowLayout&&) = delete;

  void addItem(QLayoutItem* item) override;
  int horizontalSpacing() const;
  int verticalSpacing() const;
  Qt::Orientations expandingDirections() const override;
  bool hasHeightForWidth() const override;
  int heightForWidth(int width) const override;
  int count() const override;
  QLayoutItem* itemAt(int index) const override;
  QSize minimumSize() const override;
  void setGeometry(const QRect& rect) override;
  QSize sizeHint() const override;
  QLayoutItem* takeAt(int index) override;

private:
  int doLayout(const QRect& rect, bool test_only) const;
  int smartSpacing(QStyle::PixelMetric pm) const;

  QList<QLayoutItem*> m_item_list;
  int m_hSpace;
  int m_vSpace;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_FLOW_LAYOUT_H_
