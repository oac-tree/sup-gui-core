/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_COMPONENT_OBJECTS_SCROLL_AREA_MIRROR_CONTROLLER_H_
#define SUP_GUI_COMPONENT_OBJECTS_SCROLL_AREA_MIRROR_CONTROLLER_H_

#include <QObject>
#include <Qt>
#include <vector>

class QAbstractScrollArea;

namespace sup::gui
{

/**
 * @brief The ScrollAreaMirrorController class synchronises the scroll position of multiple
 * QAbstractScrollArea (or QAbstractItemView) instances.
 *
 * After registering two or more areas via AddScrollArea(), any change to the horizontal or
 * vertical scrollbar of one area is immediately mirrored to all other registered areas.
 * Synchronisation can be temporarily suspended with SetActive(false).
 */
class ScrollAreaMirrorController : public QObject
{
  Q_OBJECT

public:
  explicit ScrollAreaMirrorController(QObject* parent_object = nullptr);
  ~ScrollAreaMirrorController() override;

  ScrollAreaMirrorController(const ScrollAreaMirrorController&) = delete;
  ScrollAreaMirrorController& operator=(const ScrollAreaMirrorController&) = delete;
  ScrollAreaMirrorController(ScrollAreaMirrorController&&) = delete;
  ScrollAreaMirrorController& operator=(ScrollAreaMirrorController&&) = delete;

  /**
   * @brief Adds a scroll area to the synchronisation group.
   *
   * Does nothing if @p area is nullptr or already registered.
   */
  void AddScrollArea(QAbstractScrollArea* area);

  /**
   * @brief Removes a scroll area from the synchronisation group.
   *
   * Does nothing if @p area is nullptr or not registered.
   */
  void RemoveScrollArea(QAbstractScrollArea* area);

  /**
   * @brief Enables or disables scroll mirroring without unregistering areas.
   */
  void SetActive(bool value);

  /**
   * @brief Returns true when mirroring is active.
   */
  bool IsActive() const;

private:
  void OnScrollBarValueChanged(QAbstractScrollArea* source, Qt::Orientation orientation, int value);

  std::vector<QAbstractScrollArea*> m_areas;
  bool m_is_active{true};
  bool m_is_propagating{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENT_OBJECTS_SCROLL_AREA_MIRROR_CONTROLLER_H_
