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

#ifndef SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
#define SUP_GUI_APP_APP_CONTEXT_MANAGER_H_

#include <map>

class QWidget;

namespace sup::gui
{

class AppContext;

/**
 * @brief The AppContextManager class stores correspondence of focus widgets to context.
 */
class AppContextManager
{
public:
  /**
   * @brief Registers given widget using unique identifier and returns associated context to the
   * user.
   *
   * If widget was already registered, return corresponding conext.
   */
  AppContext RegisterWidgetUniqueId(const QWidget* widget);

  /**
   * @brief Returns context for widget.
   *
   * Context will be empty, if widget wasn't registered yet.
   */
  AppContext GetContext(const QWidget* widget) const;

  /**
   * @brief Returns registered widget for give context.
   */
  const QWidget* GetWidget(const AppContext& context) const;

  /**
   * @brief Checks if widget was registered.
   */
  bool HasContext(const QWidget* widget) const;

private:
  std::map<const QWidget*, AppContext> m_widget_to_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
