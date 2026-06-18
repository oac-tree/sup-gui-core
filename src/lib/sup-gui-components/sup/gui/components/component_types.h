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

#ifndef SUP_GUI_COMPONENTS_COMPONENT_TYPES_H_
#define SUP_GUI_COMPONENTS_COMPONENT_TYPES_H_

//! @file
//! Collection of mixed types for various widgets

#include <cstdint>

namespace sup::gui
{

/**
 * @brief The WaveformDisplayMode enum defines various ways how multiple waveform can be persented
 * on a single canvas.
 */
enum class WaveformDisplayMode : std::uint8_t
{
  kDisplayAll,       //!< display all waveforms on a single canvas
  kDisplaySelected,  //!< display currently selected canvas, hide others
  kTotalCount,
};

/**
 * @brief The ConfigurationViewMode enum gives various ways how configuration can be presented in a
 * view.
 */
enum class ConfigurationViewMode : std::uint8_t
{
  kTree,  //!< display configuration in a tree view
  kJSON,  //!< display configuration in a JSON view
  kFlat,  //!< display configuration in a flat view
  kTotalCount
};

/**
 * @brief The ComparePanelTypes enum defines various types of panels in a comparison view.
 */
enum class ComparePanelTypes : std::uint8_t
{
  //!< default panel appearance (split/close buttons and combo selector)
  kDefault,
  //!< panel is intended to be used as a reference panel (same as above, and reference mark)
  kReference
};

/**
 * @brief The ComparePanelStatus enum defines various states of a panel in a comparison view.
 */
enum class ComparePanelState : std::uint8_t
{
  kIsReference,  //! panel is marked as reference
  kActivePanel,  //! panel is currently under user focus
  kLastPanel,    //! panel is the last editor in a splitter
  size           //! obligatory total count
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_COMPONENT_TYPES_H_
