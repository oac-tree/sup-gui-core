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

#ifndef SUP_GUI_DTOEDITOR_DTO_COMPOSER_VIEW_H_
#define SUP_GUI_DTOEDITOR_DTO_COMPOSER_VIEW_H_

#include <QWidget>
#include <map>
#include <memory>

class QTabWidget;

namespace mvvm
{
class SessionModelInterface;
class SessionItem;
}

namespace sup::gui
{

class DtoComposerTabController;

/**
 * @brief The DtoComposerView class represents a main view to assemble AnyValue.
 *
 * @details It contains tabs with multiple instances of AnyValueEditor, belongs to MainWindow.
 */
class DtoComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoComposerView(mvvm::SessionModelInterface* model, QWidget* parent = nullptr);

  void AddAnyValue();

private:
  mvvm::SessionModelInterface* m_model{nullptr};

  QTabWidget* m_tab_widget{nullptr};
  std::unique_ptr<DtoComposerTabController> m_tab_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_DTOEDITOR_DTO_COMPOSER_VIEW_H_
