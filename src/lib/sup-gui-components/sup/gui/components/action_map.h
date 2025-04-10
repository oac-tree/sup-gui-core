/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_COMPONENTS_ACTION_MAP_H_
#define SUP_GUI_COMPONENTS_ACTION_MAP_H_

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QAction>
#include <algorithm>
#include <map>

namespace sup::gui
{

/**
 * @brief The ActionMap class is a simple map with actions stored against keys.
 *
 * Provides additional checks for existing keys and existing actions.
 */
template <typename ActionKeyT>
class ActionMap
{
public:
  void Add(ActionKeyT key, QAction* action)
  {
    if (GetAction(key))
    {
      throw RuntimeException("Key is already registered");
    }

    if (!action)
    {
      throw RuntimeException("Can't register null action");
    }

    m_action_map[key] = action;
  }

  /**
   * @brief Returns list of actions according to provided keys.
   */
  QList<QAction*> GetActions(const std::vector<ActionKeyT>& action_keys) const
  {
    QList<QAction*> result;
    std::transform(action_keys.begin(), action_keys.end(), std::back_inserter(result),
                   [this](auto element) { return GetAction(element); });

    return result;
  }

  /**
   * @brief Returns action for given key.
   */
  QAction* GetAction(ActionKeyT key) const
  {
    auto iter = m_action_map.find(key);
    return iter == m_action_map.end() ? nullptr : iter->second;
  }

  /**
   * @brief Returns key for given action.
   *
   * Throws if the key can't be found.
   */
  ActionKeyT GetKey(QAction* action)
  {
    auto iter = std::find_if(m_action_map.begin(), m_action_map.end(),
                             [action](auto element) { return element.second == action; });

    if (iter == m_action_map.end())
    {
      throw RuntimeException("Can't convert QAction to action key");
    }
    return iter->first;
  }

  /**
   * @brief Returns the number of registered actions.
   */
  int GetSize() const { return static_cast<int>(m_action_map.size()); }

private:
  std::map<ActionKeyT, QAction*> m_action_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ACTION_MAP_H_
