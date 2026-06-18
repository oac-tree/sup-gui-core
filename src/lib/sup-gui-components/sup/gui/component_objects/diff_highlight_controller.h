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

#ifndef SUP_GUI_COMPONENT_OBJECTS_DIFF_HIGHLIGHT_CONTROLLER_H_
#define SUP_GUI_COMPONENT_OBJECTS_DIFF_HIGHLIGHT_CONTROLLER_H_

#include <QObject>

class QAbstractItemModel;
class QModelIndex;

namespace sup::gui
{

/**
 * @brief The DiffHighlightController class highlights the cells of a tree that differ from a
 * reference tree.
 *
 * The controller works with two models that share the same branch/children layout: a reference
 * model (A) and a highlight model (B). It monitors the DisplayRole data changes in both models;
 * whenever a cell changes, the corresponding cell in the other model is located by its row/column
 * path, and the BackgroundRole of the cell in the highlight model (B) is set to red when the two
 * display values differ (and cleared when they are equal).
 */
class DiffHighlightController : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param reference_model The reference model (A).
   * @param highlight_model The model whose cells get highlighted (B).
   * @param parent_object Optional parent object for Qt ownership.
   */
  explicit DiffHighlightController(QAbstractItemModel* reference_model,
                                   QAbstractItemModel* highlight_model,
                                   QObject* parent_object = nullptr);
  ~DiffHighlightController() override;

  DiffHighlightController(const DiffHighlightController&) = delete;
  DiffHighlightController& operator=(const DiffHighlightController&) = delete;
  DiffHighlightController(DiffHighlightController&&) = delete;
  DiffHighlightController& operator=(DiffHighlightController&&) = delete;

  /**
   * @brief Enables or disables highlighting.
   */
  void SetActive(bool value);

  /**
   * @brief Returns true when highlighting is active.
   */
  bool IsActive() const;

  /**
   * @brief Visits all cells of the highlight model and sets their initial highlight, depending on
   * the reference model.
   *
   * Both models are walked simultaneously (positionally), which avoids reconstructing an index
   * path for every cell.
   */
  void ProcessAll();

private:
  /**
   * @brief Reacts to a DisplayRole change of a range of cells in the reference model.
   */
  void ProcessReferenceChange(const QModelIndex& top_left, const QModelIndex& bottom_right);

  /**
   * @brief Reacts to a DisplayRole change of a range of cells in the highlight model.
   */
  void ProcessHighlightChange(const QModelIndex& top_left, const QModelIndex& bottom_right);

  /**
   * @brief Recursively walks the two models in parallel under the given parents and updates the
   * highlight of every cell.
   */
  void ProcessBranch(const QModelIndex& reference_parent, const QModelIndex& highlight_parent);

  /**
   * @brief Compares the display values of the two corresponding cells and updates the background
   * of the cell in the highlight model.
   */
  void UpdateHighlightCell(const QModelIndex& reference_index, const QModelIndex& highlight_index);

  QAbstractItemModel* m_reference_model{nullptr};
  QAbstractItemModel* m_highlight_model{nullptr};
  bool m_is_active{true};
  bool m_is_updating{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENT_OBJECTS_DIFF_HIGHLIGHT_CONTROLLER_H_
