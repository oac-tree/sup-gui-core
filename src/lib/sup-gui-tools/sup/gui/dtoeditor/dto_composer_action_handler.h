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

#ifndef SUP_GUI_DTOEDITOR_DTO_COMPOSER_ACTION_HANDLER_H_
#define SUP_GUI_DTOEDITOR_DTO_COMPOSER_ACTION_HANDLER_H_

#include <QObject>

namespace sup::gui
{

/**
 * @brief The DtoComposerActionHandler class provides a logic to handle main actions of
 * DtoComposerView.
 *
 * @details Actions are related to multiple AnyValueItem editing and can populate the main
 * application menubar, tabs context menu, etc.
 */
class DtoComposerActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit DtoComposerActionHandler(QObject* parent = nullptr);
};

}  // namespace sup::gui

#endif  // SUP_GUI_DTOEDITOR_DTO_COMPOSER_ACTION_HANDLER_H_
