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

#ifndef SUP_GUI_COMPONENT_OBJECTS_TREE_HIGHLIGHT_CONTROLLER_H_
#define SUP_GUI_COMPONENT_OBJECTS_TREE_HIGHLIGHT_CONTROLLER_H_

#include <QObject>
#include <memory>
#include <vector>

class QAbstractItemModel;

namespace sup::gui
{

class DiffHighlightController;

/**
 * @brief The TreeHighlightController class highlights the differences of several models against a
 * common reference model.
 *
 * Models are registered with AddViewModel(); one of them is designated as the reference via
 * SetReferenceViewModel(). For every non-reference model a DiffHighlightController is created,
 * which highlights the cells that differ from the reference. All registered models are expected to
 * share the same branch/children layout.
 *
 * Whenever the set of models or the reference changes, the underlying DiffHighlightController
 * instances are recreated from scratch, which avoids tracking the previous reference.
 */
class TreeHighlightController : public QObject
{
  Q_OBJECT

public:
  explicit TreeHighlightController(QObject* parent_object = nullptr);
  ~TreeHighlightController() override;

  TreeHighlightController(const TreeHighlightController&) = delete;
  TreeHighlightController& operator=(const TreeHighlightController&) = delete;
  TreeHighlightController(TreeHighlightController&&) = delete;
  TreeHighlightController& operator=(TreeHighlightController&&) = delete;

  /**
   * @brief Adds a model to the group.
   *
   * Does nothing if @p model is nullptr or already registered.
   */
  void AddViewModel(QAbstractItemModel* model);

  /**
   * @brief Removes a model from the group.
   *
   * Does nothing if @p model is nullptr or not registered. If the removed model was the reference,
   * the reference is reset.
   */
  void RemoveViewModel(QAbstractItemModel* model);

  /**
   * @brief Sets the model used as the reference for highlighting.
   *
   * Recreates the underlying DiffHighlightController instances for all other models and performs an
   * initial highlight pass.
   */
  void SetReferenceViewModel(QAbstractItemModel* model);

  /**
   * @brief Returns the model currently used as the reference (nullptr if none).
   */
  QAbstractItemModel* GetReferenceViewModel() const;

  /**
   * @brief Enables or disables highlighting.
   */
  void SetActive(bool value);

  /**
   * @brief Returns true when highlighting is active.
   */
  bool IsActive() const;

private:
  void RecreateDiffControllers();

  std::vector<QAbstractItemModel*> m_view_models;
  QAbstractItemModel* m_reference_model{nullptr};
  std::vector<std::unique_ptr<DiffHighlightController>> m_diff_controllers;
  bool m_is_active{true};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENT_OBJECTS_TREE_HIGHLIGHT_CONTROLLER_H_
