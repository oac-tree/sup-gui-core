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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_

#include <QWidget>
#include <memory>

class QTabWidget;

namespace mvvm
{
class ISessionModel;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class DtoComposerTabController;
class DtoComposerActions;
class DtoComposerActionHandler;

/**
 * @brief The DtoComposerView class represents a main view to assemble AnyValue.
 *
 * It contains tabs with multiple instances of AnyValueEditor, belongs to MainWindow.
 */
class DtoComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoComposerView(QWidget* parent_widget = nullptr);
  ~DtoComposerView() override;

  void SetModel(mvvm::ISessionModel* model);

private:
  void SetupConnections();
  void SummonContextMenu(const QPoint& point);

  mvvm::ISessionModel* m_model{nullptr};

  QTabWidget* m_tab_widget{nullptr};
  std::unique_ptr<DtoComposerTabController> m_tab_controller;
  DtoComposerActions* m_actions{nullptr};
  DtoComposerActionHandler* m_action_handler;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPOSER_VIEW_H_
