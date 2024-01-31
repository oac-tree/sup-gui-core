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

#ifndef SUP_GUI_DTOEDITOR_DTO_COMPOSER_TAB_CONTROLLER_H_
#define SUP_GUI_DTOEDITOR_DTO_COMPOSER_TAB_CONTROLLER_H_

#include <mvvm/signals/event_types.h>

#include <QObject>
#include <map>
#include <memory>

class QTabWidget;

namespace mvvm
{
class SessionModelInterface;
template <typename T>
class ModelListener;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueEditorWidget;

/**
 * @brief The DtoComposerTabController class controlls adding/removal of QTabWidget's tabs when
 * AnyValueItem containers are being added or removed from the model.
 *
 * @details It is expected that the model contains a number of top level container items. Adding a
 * new container will lead to appearance of a new tab. Container removal will trigger tab removal.
 */
class DtoComposerTabController : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main controller constructor.
   *
   * @param model The application model.
   * @param tab_widget QTabWidget to control
   * @param parent QObject parent
   */
  DtoComposerTabController(mvvm::SessionModelInterface* model, QTabWidget* tab_widget,
                           QObject* parent = nullptr);
  ~DtoComposerTabController() override;

  AnyValueEditorWidget* GetWidgetForItem(const mvvm::SessionItem*container);

private:
  /**
   * @brief Create necessary tabs to reflect initial state of the model;
   */
  void InitTabs();

  /**
   * @brief Creates new tab when top-level container is inserted in the model.
   */
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event);

  /**
   * @brief Removes corresponding tab when top-level container is inserted in the model.
   */
  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event);

  /**
   * @brief Inserts widget tab corresponding to a given container.
   */
  void InsertAnyValueItemContainerTab(mvvm::SessionItem* container, int index);

  mvvm::SessionModelInterface* m_model{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  std::unique_ptr<mvvm::ModelListener<mvvm::SessionModelInterface>> m_listener;

  //!< correspondance of AnyValueItem container to AnyValueEditorWidget
  std::map<const mvvm::SessionItem*, AnyValueEditorWidget*> m_widget_map;
};

}  // namespace sup::gui

#endif  // SUP_GUI_DTOEDITOR_DTO_COMPOSER_TAB_CONTROLLER_H_
