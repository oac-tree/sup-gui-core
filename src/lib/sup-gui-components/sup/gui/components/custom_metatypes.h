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

#ifndef SUP_GUI_COMPONENTS_CUSTOM_METATYPES_H_
#define SUP_GUI_COMPONENTS_CUSTOM_METATYPES_H_

//! file
//! Registration of custom types in Qt meta type system to provide support in signal/slot and
//! QVariant.

#include <QList>
#include <QMetaType>

namespace sup::gui
{

void RegisterCustomMetaTypes();

}  // namespace sup::gui

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(QList<int>)
#endif  // SUP_GUI_COMPONENTS_CUSTOM_METATYPES_H_

#endif  // SUP_GUI_COMPONENTS_CUSTOM_METATYPES_H_
