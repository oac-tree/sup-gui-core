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

#ifndef SUP_GUI_APP_APP_COMMAND_CONTEXT_WIDGET_MAP_H_
#define SUP_GUI_APP_APP_COMMAND_CONTEXT_WIDGET_MAP_H_

#include <sup/gui/app/i_app_command_context_widget_map.h>

#include <map>

class QWidget;

namespace sup::gui
{

/**
 * @brief The AppCommandContextWidgetMap class is a default implementation to store the
 * correspondance of widgets to AppCommandContext.
 */
class AppCommandContextWidgetMap : public IAppCommandContextWidgetMap
{
public:
  AppCommandContextWidgetMap() = default;
  ~AppCommandContextWidgetMap() override;

  AppCommandContextWidgetMap(const AppCommandContextWidgetMap&) = delete;
  AppCommandContextWidgetMap& operator=(const AppCommandContextWidgetMap&) = delete;
  AppCommandContextWidgetMap(AppCommandContextWidgetMap&&) = delete;
  AppCommandContextWidgetMap& operator=(AppCommandContextWidgetMap&&) = delete;

  AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) override;

  void UnregisterWidgetUniqueId(const QWidget* widget) override;

  AppCommandContext GetContext(const QWidget* widget) const override;

  const QWidget* GetWidget(const AppCommandContext& context) const override;

  bool HasContext(const QWidget* widget) const override;

  std::size_t GetNumberOfRegistrations() const override;

private:
  std::map<const QWidget*, AppCommandContext> m_widget_to_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_CONTEXT_WIDGET_MAP_H_
