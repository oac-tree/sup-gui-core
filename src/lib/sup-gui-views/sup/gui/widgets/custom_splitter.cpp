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

#include "custom_splitter.h"

#include "custom_splitter_controller.h"

#include <QChildEvent>
#include <QSettings>

namespace sup::gui
{

CustomSplitter::CustomSplitter(const QString& settings_group_name, QWidget* parent_widget)
    : QSplitter(parent_widget)
    , m_controller(std::make_unique<CustomSplitterController>(settings_group_name, this))
{
}

CustomSplitter::~CustomSplitter() = default;

void CustomSplitter::ReadSettings(const read_variant_func_t& read_func)
{
  m_controller->ReadSettings(read_func);
  m_controller->StartChildrenListening();
}

void CustomSplitter::WriteSettings(const write_variant_func_t& write_func)
{
  m_controller->WriteSettings(write_func);
}

}  // namespace sup::gui
