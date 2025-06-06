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
 * These are actions related to multiple AnyValueItem editing (add/remove items in tabs).
 */
class DtoComposerActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit DtoComposerActionHandler(QObject* parent_object = nullptr);

  explicit DtoComposerActionHandler(mvvm::ISessionModel* model, QObject* parent_object = nullptr);

  /**
   * @brief Sets the model with AnyValueItem containers.
   */
  void SetModel(mvvm::ISessionModel* model);

  /**
   * @brief Remove container with given index.
   */
  void OnRemoveContainer(std::size_t container_index);

  /**
   * @brief Adds new container for AnyValueItem editing.
   */
  void OnAddNewContainer();

  /**
   * @brief Duplicate container.
   */
  void OnDuplicateContainer(std::size_t container_index);

private:
  void ValidateModel();

  mvvm::ISessionModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_COMPOSER_ACTION_HANDLER_H_
