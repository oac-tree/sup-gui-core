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

#ifndef SUP_GUI_APP_APP_CONTEXT_H_
#define SUP_GUI_APP_APP_CONTEXT_H_

#include <QString>

class QWidget;

namespace sup::gui
{

/**
 * @brief The AppContext class carries information to associate actions with certain context.
 *
 * It is used to forward proxy actions, like "copy" and "paste" actions of the main menubar, to
 * concrete actions of widgets, that are currently in focus.
 */
class AppContext
{
public:
  AppContext();

  /**
   * @brief Main constructor.
   *
   * @param context The name of the activity.
   */
  explicit AppContext(const QString& context);

  /**
   * @brief Returns context name.
   */
  QString GetContextName() const;

  bool operator==(const AppContext& other) const;
  bool operator!=(const AppContext& other) const;
  bool operator<(const AppContext& other) const;
  bool operator>=(const AppContext& other) const;

private:
  QString m_context_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_H_
