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

#ifndef SUP_GUI_MAINWINDOW_COMMON_SETTINGS_EDITOR_H_
#define SUP_GUI_MAINWINDOW_COMMON_SETTINGS_EDITOR_H_

#include <sup/gui/mainwindow/session_item_widget.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class PropertyFlatView;
class PropertyViewModel;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The CommonSettingsEditor class is intended to present CommonSettingsItem.
 */
class CommonSettingsEditor : public SessionItemWidget
{
  Q_OBJECT

public:
  explicit CommonSettingsEditor(QWidget* parent_widget = nullptr);
  ~CommonSettingsEditor() override;

  CommonSettingsEditor(const CommonSettingsEditor&) = delete;
  CommonSettingsEditor& operator=(const CommonSettingsEditor&) = delete;
  CommonSettingsEditor(CommonSettingsEditor&&) = delete;
  CommonSettingsEditor& operator=(CommonSettingsEditor&&) = delete;

  void SetItem(mvvm::SessionItem* item) override;

  const mvvm::SessionItem* GetItem() const override;

private:
  mvvm::PropertyFlatView* m_settings_view{nullptr};
  std::unique_ptr<mvvm::PropertyViewModel> m_property_view_model;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_COMMON_SETTINGS_EDITOR_H_
