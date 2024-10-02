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

#ifndef SUP_GUI_COMPONENTS_DTO_COMPOSER_TAB_CONTROLLER_H_
#define SUP_GUI_COMPONENTS_DTO_COMPOSER_TAB_CONTROLLER_H_

#include <mvvm/signals/event_types.h>

#include <QObject>
#include <functional>
#include <map>
#include <memory>

class QTabWidget;

namespace mvvm
{
class ISessionModel;
class ModelListener;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The DtoComposerTabController class controlls adding/removal of QTabWidget's tabs when
 * AnyValueItem containers are being added or removed from the model.
 *
 * It is expected that the model contains a number of top level container items. Adding a
 * new container will lead to appearance of a new tab. Container removal will trigger tab removal.
 */
class DtoComposerTabController : public QObject
{
  Q_OBJECT

public:
  using create_widget_callback_t = std::function<std::unique_ptr<QWidget>(mvvm::SessionItem*)>;

  /**
   * @brief Main controller constructor.
   *
   * @param model The application model.
   * @param create_widget_callback_t A callback to create widget for given item.
   * @param tab_widget QTabWidget to control
   * @param parent QObject parent
   */
  DtoComposerTabController(mvvm::ISessionModel* model, create_widget_callback_t callback,
                           QTabWidget* tab_widget, QObject* parent = nullptr);
  ~DtoComposerTabController() override;

  QWidget* GetWidgetForItem(const mvvm::SessionItem* container);

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

  /**
   * @brief Removes tabs when model is reset.
   */
  void OnModelAboutToBeResetEvent(const mvvm::ModelAboutToBeResetEvent& event);

  /**
   * @brief Regenerates tabs at the end of the reset event.
   *
   * When project is loaded from disk, root item can be replaced with non-empty root item.
   */
  void OnModelResetEvent(const mvvm::ModelResetEvent& event);

  /**
   * @brief Clear QTabWidget from all content.
   */
  void Clear();

  mvvm::ISessionModel* m_model{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;

  //!< correspondance of AnyValueItem container to AnyValueEditorWidget
  std::map<const mvvm::SessionItem*, QWidget*> m_widget_map;

  create_widget_callback_t m_create_widget_callback;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_COMPOSER_TAB_CONTROLLER_H_
