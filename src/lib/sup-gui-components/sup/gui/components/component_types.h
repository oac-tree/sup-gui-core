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

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_COMPONENT_TYPES_H_
