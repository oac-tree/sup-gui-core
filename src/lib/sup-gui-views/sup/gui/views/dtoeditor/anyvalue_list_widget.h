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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_ANYVALUE_LIST_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_ANYVALUE_LIST_WIDGET_H_

#include <QWidget>

class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The AnyValueListWidget class represents a list of AnyValue with simple editable names.
 */
class AnyValueListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueListWidget(QWidget* parent_widget = nullptr);

private:
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateProvider() const;

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_ANYVALUE_LIST_WIDGET_H_
