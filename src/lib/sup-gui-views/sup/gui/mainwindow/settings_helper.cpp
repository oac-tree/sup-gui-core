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

#include "settings_helper.h"

#include <sup/gui/model/settings_model.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QSettings>
#include <stack>

namespace sup::gui
{

const SettingsModel &GetGlobalSettings()
{
  static const SettingsModel model;
  return model;
}

void SaveSettingsInPersistentStorage(const SettingsModel &model)
{
  QSettings settings;

  auto xml_string = mvvm::utils::ToXMLString(*model.GetRootItem());
  settings.setValue(kRootSettingsModelName, QString::fromStdString(xml_string));
}

void LoadSettingsFromPersistentStorage(SettingsModel &model)
{
  const QSettings settings;

  // If the QSettings file contains a record, we will fully rebuild the settings model with the
  // content, stored in the file. This is not a very clean approach, since our C++ model
  // might not match what was stored in a file a while ago. TODO find a way to update the model only
  // with meaningful content.
  if (settings.contains(kRootSettingsModelName))
  {
    auto str = settings.value(kRootSettingsModelName).toString().toStdString();
    auto root_item = mvvm::utils::SessionItemFromXMLString(str);
    model.ReplaceRootItem(std::move(root_item));
  }
}

void ReadGlobalSettings()
{
  LoadSettingsFromPersistentStorage(const_cast<SettingsModel &>(GetGlobalSettings()));
}

void WriteSettingsToPersistentStorage(const mvvm::ISessionModel &model, write_variant_func_t func)
{
  QString model_key(QString::fromStdString(model.GetType()));

  struct Node
  {
    const mvvm::SessionItem *item{nullptr};
    QString key;
  };

  std::stack<Node> stack;
  stack.push({model.GetRootItem(), model_key});

  while (!stack.empty())
  {
    auto item = stack.top().item;
    auto top_key = stack.top().key;
    QString item_key = top_key;
    if (item != model.GetRootItem())
    {
      item_key += "/" + QString::fromStdString(item->GetDisplayName());
    }

    if (item->HasData())
    {
      const QVariant variant = mvvm::GetQtVariant(item->Data());
      func(item_key, variant);
    }

    stack.pop();

    auto children = item->GetAllItems();
    // push in reverse order to provide correct child order processing
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push({*it, item_key});
    }
  }
}

void ReadSettingsFromPersistentStorage(mvvm::ISessionModel &model, read_variant_func_t func)
{
  (void)model;
  (void)func;
}

}  // namespace sup::gui
