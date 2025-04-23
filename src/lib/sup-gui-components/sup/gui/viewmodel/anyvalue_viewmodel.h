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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_VIEWMODEL_ANYVALUE_VIEWMODEL_H_
#define SUP_GUI_VIEWMODEL_ANYVALUE_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace sup::gui
{

/**
 * @brief The AnyValueViewModel class is a view model to show AnyValueItem with editable display
 * name, value columnt, and type column.
 */
class AnyValueViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit AnyValueViewModel(mvvm::ISessionModel* model, QObject* parent = nullptr);
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWMODEL_ANYVALUE_VIEWMODEL_H_
