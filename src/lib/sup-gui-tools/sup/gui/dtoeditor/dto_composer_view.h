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

class QTabWidget;

namespace mvvm
{
class ApplicationModel;
}

namespace sup::gui
{

/**
 * @brief The DtoComposerView class represents a main view to assemble AnyValue.
 *
 * @details It contains tabs with multiple instances of AnyValueEditor, belongs to MainWindow.
 */
class DtoComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit DtoComposerView(mvvm::ApplicationModel* model, QWidget* parent = nullptr);

private:
  QTabWidget* m_tab_widget{nullptr};
  mvvm::ApplicationModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_DTOEDITOR_DTO_COMPOSER_VIEW_H_
