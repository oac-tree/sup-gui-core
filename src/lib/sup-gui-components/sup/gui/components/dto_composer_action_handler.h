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

#ifndef SUP_GUI_COMPONENTS_DTO_COMPOSER_ACTION_HANDLER_H_
#define SUP_GUI_COMPONENTS_DTO_COMPOSER_ACTION_HANDLER_H_

#include <QObject>

namespace mvvm
{
class ISessionModel;
}

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
  explicit DtoComposerActionHandler(mvvm::ISessionModel* model, QObject* parent = nullptr);

  /**
   * @brief Remove container with given index.
   */
  void OnRemoveContainer(int container_index);

  /**
   * @brief Adds new container for AnyValueItem editing.
   */
  void OnAddNewContainer();

  /**
   * @brief Duplicate container.
   */
  void OnDuplicateContainer(int container_index);

private:
  mvvm::ISessionModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_COMPOSER_ACTION_HANDLER_H_
