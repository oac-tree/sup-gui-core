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

#include "splittable_editor_controller.h"

#include "splittable_combo_panel.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QSplitter>

namespace sup::gui
{

SplittableEditorController::SplittableEditorController(QSplitter* splitter, QObject* parent_object)
    : QObject(parent_object), m_splitter(splitter)
{
  if (m_splitter == nullptr)
  {
    throw RuntimeException("Splitter is not initialized");
  }
}

QWidget* SplittableEditorController::CreatePanel(QWidget* after_widget)
{
  (void)after_widget;
  return nullptr;
}

void SplittableEditorController::ClosePanel(QWidget* widget_to_close)
{
  (void)widget_to_close;
}

QWidget* SplittableEditorController::GetFocusWidget()
{
  return nullptr;
}

std::unique_ptr<SplittableComboPanel> SplittableEditorController::CreateSplittableComboPanel()
{
  return {};
}

}  // namespace sup::gui
