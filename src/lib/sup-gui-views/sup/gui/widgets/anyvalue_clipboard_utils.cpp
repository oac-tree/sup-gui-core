/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#include "anyvalue_clipboard_utils.h"

#include <sup/gui/model/anyvalue_utils.h>

#include <sup/dto/anyvalue.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>

namespace
{

// Quick shallow check for a valid AnyValue JSON
bool IsNotAnyValue(const QJsonArray& array)
{
  // AnyValues are encoded in a 3 object array
  const auto is_not_object = [](const auto& value) { return !value.isObject(); };
  return array.size() != 3 || std::any_of(array.begin(), array.end(), is_not_object);
}
bool IsNotAnyValue(const QJsonValue& val)
{
  return !val.isArray() || IsNotAnyValue(val.toArray());
}
bool IsNotAnyValue(const QJsonDocument& doc)
{
  return !doc.isArray() || IsNotAnyValue(doc.array());
}

// Quick shallow check for a valid AnyValue JSON List
bool IsNotAnyValueList(const QJsonDocument& doc)
{
  const auto is_not_anyvalue = [](const auto& value) { return IsNotAnyValue(value); };
  return !doc.isArray() || std::any_of(doc.array().begin(), doc.array().end(), is_not_anyvalue);
}

std::vector<sup::dto::AnyValue> ParseClipboardAnyValueJSON(const QString& json_str)
{
  const auto json{QJsonDocument::fromJson(json_str.toUtf8())};
  if (json.isNull())
  {
    return {};
  }

  if (!IsNotAnyValue(json))
  {
    // Try to parse the clipboard content as a single AnyValue
    return {sup::gui::AnyValueFromJSONString(json.toJson().toStdString())};
  }
  else if (!IsNotAnyValueList(json))
  {
    // Try to parse the clipboard content as a list of AnyValue
    const auto anyvalue_from_json = [](const auto& value) {
      return sup::gui::AnyValueFromJSONString(
          QJsonDocument(value.toArray()).toJson().toStdString());
    };

    std::vector<sup::dto::AnyValue> result;
    const auto& array{json.array()};
    std::transform(array.begin(), array.end(), std::back_inserter(result), anyvalue_from_json);
    return result;
  }

  return {};
}

}  // namespace

namespace sup::gui
{

sup::dto::AnyValue ClipboardGetAnyvalue(QClipboard::Mode mode, QClipboard* clipboard)
{
  const auto anyvalues{ClipboardGetAnyvalueList(mode, clipboard)};
  if (anyvalues.empty())
  {
    return {};
  }

  return anyvalues.front();
}

std::vector<sup::dto::AnyValue> ClipboardGetAnyvalueList(QClipboard::Mode mode,
                                                         QClipboard* clipboard)
{
  if (clipboard == nullptr)
  {
    return {};
  }

  const auto anyvalue_json{clipboard->text(mode)};
  if (anyvalue_json.isEmpty())
  {
    return {};
  }

  try
  {
    return ParseClipboardAnyValueJSON(anyvalue_json);
  }
  catch (...)
  {
    return {};
  }
}

void ClipboardSetAnyvalue(const sup::dto::AnyValue& anyvalue, QClipboard::Mode mode,
                          QClipboard* clipboard)
{
  if (clipboard == nullptr)
  {
    return;
  }

  clipboard->setText(QString::fromStdString(AnyValueToJSONString(anyvalue)), mode);
}

void ClipboardSetAnyvalueList(const std::vector<sup::dto::AnyValue>& anyvalues,
                              QClipboard::Mode mode, QClipboard* clipboard)
{
  if (anyvalues.empty())
  {
    return;
  }
  if (anyvalues.size() == 1)
  {
    ClipboardSetAnyvalue(anyvalues.front(), mode, clipboard);
    return;
  }

  if (clipboard == nullptr)
  {
    return;
  }

  QJsonArray array;
  for (const auto& value : anyvalues)
  {
    const auto json{
        QJsonDocument::fromJson(QByteArray::fromStdString(AnyValueToJSONString(value)))};
    if (json.isNull())
    {
      return;
    }
    array.append(json.array());
  }

  clipboard->setText(QJsonDocument(array).toJson(QJsonDocument::Compact), mode);
}

}  // namespace sup::gui
