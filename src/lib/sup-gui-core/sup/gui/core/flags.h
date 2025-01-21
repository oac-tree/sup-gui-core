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

#ifndef SUP_GUI_CORE_FLAGS_H_
#define SUP_GUI_CORE_FLAGS_H_

#include <algorithm>
#include <vector>

namespace sup::gui
{

/**
 * @brief The Flags class is a simple container for flags defined by the enum class.
 *
 * @tparam EnumT Enum class type.
 */
template <typename EnumT>
class Flags
{
public:
  Flags() = default;

  /**
   * @brief C-tor to initialize flags from single value.
   *
   * Intentially allows implicit conversion.
   */
  Flags(EnumT flag) : Flags(std::vector<EnumT>{flag}) {}

  /**
   * @brief C-tor to initialize flags vector of values.
   *
   * Intentially allows implicit conversion.
   */
  Flags(const std::vector<EnumT>& flags) : m_flags(flags) {}

  /**
   * @brief Checks if given flag is present.
   */
  bool HasFlag(EnumT flag) const
  {
    return std::find(m_flags.cbegin(), m_flags.cend(), flag) != m_flags.cend();
  }

  /**
   * @brief Sets flags from given values, discards previous flags.
   */
  void SetFlags(std::vector<EnumT> flags) { m_flags = std::move(flags); }

  /**
   * @brief Adds a flag to the list of flags.
   *
   * If flag already exists, will do nothing.
   */
  void SetFlag(EnumT flag)
  {
    if (!HasFlag(flag))
    {
      m_flags.push_back(flag);
    }
  }

  /**
   * @brief Remove the flag from the list of flags.
   *
   * If the flag didn't exist, will do nothing.
   */
  void UnsetFlag(EnumT flag)
  {
    auto iter = std::find(std::begin(m_flags), std::end(m_flags), flag);
    if (iter != m_flags.end())
    {
      m_flags.erase(iter);
    }
  }

  /**
   * @brief Returns number of flags.
   */
  std::size_t GetFlagCount() const { return m_flags.size(); }

private:
  std::vector<EnumT> m_flags;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_FLAGS_H_
