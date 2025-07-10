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

#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QSettings>
#include <stack>

namespace sup::gui
{

namespace
{

struct Node
{
  mvvm::SessionItem *item{nullptr};
  QString key;
};

/**
 * @brief Returns a key representing property item in a section.
 */
QString GetItemKey(const QString &section, const mvvm::SessionItem &item)
{
  QString result = section;
  if (item.GetDisplayName() == mvvm::constants::kRootItemName)
  {
    return result;
  }

  result += "/" + QString::fromStdString(item.GetDisplayName());
  result.replace(" ", "_");

  return result;
}
}  // namespace

void AssignStringBasedVariant(const QVariant &variant, mvvm::SessionItem &item)
{
  if (!variant.isValid())
  {
    return;
  }

  auto item_data = item.Data();
  // converting stored string to a proper type
  item.SetData(
      mvvm::utils::ValueFromString(mvvm::GetTypeCode(item_data), variant.toString().toStdString()));
}

void WriteApplicationSettings(const mvvm::ISessionModel &model, write_variant_func_t func)
{
  const QString model_key(QString::fromStdString(model.GetType()));

  std::stack<Node> stack;
  stack.push({model.GetRootItem(), model_key});

  while (!stack.empty())
  {
    auto [item, top_key] = stack.top();
    const QString item_key = GetItemKey(top_key, *item);

    if (item->HasData())
    {
      const QVariant variant = mvvm::GetQtVariant(item->Data());
      func(item_key, variant);
    }

    stack.pop();

    auto children = item->GetAllItems();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push({*it, item_key});
    }
  }
}

void WriteApplicationSettings(const mvvm::ISessionModel &model)
{
  WriteApplicationSettings(model, GetSettingsWriteFunc());
}

void ReadSettingsFromPersistentStorage(mvvm::ISessionModel &model, read_variant_func_t func)
{
  const QString model_key(QString::fromStdString(model.GetType()));

  std::stack<Node> stack;
  stack.push({model.GetRootItem(), model_key});

  while (!stack.empty())
  {
    auto [item, top_key] = stack.top();
    const QString item_key = GetItemKey(top_key, *item);

    if (item->HasData())
    {
      AssignStringBasedVariant(func(item_key), *item);
    }

    stack.pop();

    auto children = item->GetAllItems();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push({*it, item_key});
    }
  }
}

void ReadApplicationSettings(mvvm::ISessionModel &model)
{
  const QSettings settings;
  if (!settings.childGroups().contains(QString::fromStdString(model.GetType())))
  {
    return;
  }
  ReadSettingsFromPersistentStorage(model, GetSettingsReadFunc());
}

}  // namespace sup::gui
