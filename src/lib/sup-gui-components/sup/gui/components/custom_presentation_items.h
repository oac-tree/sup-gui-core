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

#ifndef SUP_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
#define SUP_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_

#include <mvvm/providers/default_data_presentation_item.h>

#include <memory>

namespace mvvm
{
class BooleanDataPresentationItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueScalarItem;

/**
 * @brief The AnyValueScalarValuePresentationItem class presents the value of AnyValueScalarItem.
 *
 * @details A boolean value is shown as a checkbox, other values as a default editable cell. The
 * choice follows the current data type, so the cell stays usable after the scalar type change.
 * Presentation-only roles (background, foreground, decoration) are kept for any data type.
 */
class AnyValueScalarValuePresentationItem : public mvvm::AbstractPresentationItem
{
public:
  explicit AnyValueScalarValuePresentationItem(AnyValueScalarItem* item);
  ~AnyValueScalarValuePresentationItem() override;

  AnyValueScalarValuePresentationItem(const AnyValueScalarValuePresentationItem&) = delete;
  AnyValueScalarValuePresentationItem& operator=(const AnyValueScalarValuePresentationItem&) =
      delete;
  AnyValueScalarValuePresentationItem(AnyValueScalarValuePresentationItem&&) = delete;
  AnyValueScalarValuePresentationItem& operator=(AnyValueScalarValuePresentationItem&&) = delete;

  QVariant Data(mvvm::role_t qt_role) const override;

  bool SetData(const QVariant& data, mvvm::role_t qt_role) override;

  bool IsEditable() const override;

  bool IsCheckable() const override;

  bool IsPresentationOnlyRole(mvvm::role_t qt_role) const override;

  QVector<mvvm::role_t> GetQtRolesToEmit(mvvm::role_t data_role) const override;

private:
  //! Returns the presentation matching the current data type.
  const mvvm::AbstractPresentationItem& GetPresentation() const;

  //! Returns the presentation matching the current data type.
  mvvm::AbstractPresentationItem& GetPresentation();

  std::unique_ptr<mvvm::DefaultDataPresentationItem> m_default_presentation;
  std::unique_ptr<mvvm::BooleanDataPresentationItem> m_boolean_presentation;
};

/**
 * @brief The AnyValueScalarTypePresentationItem class presents the type name of AnyValueScalarItem.
 *
 * @details The cell shows the type name and is edited with a combo of scalar type names. A new
 * selection changes the scalar type. Presentation-only roles are kept as in the base class.
 */
class AnyValueScalarTypePresentationItem : public mvvm::DefaultDataPresentationItem
{
public:
  explicit AnyValueScalarTypePresentationItem(AnyValueScalarItem* item);

  QVariant Data(mvvm::role_t qt_role) const override;

  /**
   * @brief Sets the data for the given Qt role.
   *
   * @details The edit role expects a ComboProperty. The selection of the current type does nothing.
   */
  bool SetData(const QVariant& data, mvvm::role_t qt_role) override;

  /**
   * @brief Returns true if the scalar is editable and has a type.
   */
  bool IsEditable() const override;

  bool IsCheckable() const override;

private:
  AnyValueScalarItem* m_scalar_item{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
