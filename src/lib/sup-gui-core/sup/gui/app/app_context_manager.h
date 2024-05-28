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
#include <vector>

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

  void RegisterContext(const QWidget* widget, const AppContext& context);

  std::vector<AppContext> GetContext(const QWidget* widget) const;

private:
  std::map<const QWidget*, std::vector<AppContext>> m_widget_to_context;

};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
