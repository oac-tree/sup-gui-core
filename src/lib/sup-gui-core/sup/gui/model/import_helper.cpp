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

#include "import_helper.h"

#include "anyvalue_conversion_utils.h"
#include "anyvalue_item.h"
#include "anyvalue_utils.h"
#include "configuration_item.h"

#include <sup/gui/core/message_event.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/utils/file_utils.h>

#include <sup/dto/anyvalue.h>

namespace sup::gui
{

std::unique_ptr<AnyValueItem> CreateAnyValueItemFromFile(
    const std::string& file_name, const std::function<void(const MessageEvent&)>& report_error)
{
  try
  {
    auto anyvalue = AnyValueFromJSONFile(file_name);
    return CreateAnyValueItem(anyvalue);
  }
  catch (const std::exception& ex)
  {
    MessageEvent event;
    event.title = "Import failed";
    event.text = "Can't create AnyValue from file [" + file_name + "]";
    event.informative = "Exception was thrown";
    event.detailed = ex.what();
    report_error(event);
  }

  return {};
}

void AppendConfigurationFromFile(const std::string& file_name,
                                 const std::function<void(const MessageEvent&)>& report_error,
                                 mvvm::SessionItem& container)
{
  auto anyvalue_item = CreateAnyValueItemFromFile(file_name, report_error);
  if (!anyvalue_item)
  {
    return;
  }

  auto configuration_item = std::make_unique<ConfigurationItem>();
  (void)configuration_item->InsertItem(std::move(anyvalue_item), mvvm::TagIndex::Append());

  // name of the configuration is the same as file name without extension
  auto configuration_name = mvvm::utils::GetPathStem(file_name);
  (void)configuration_item->SetDisplayName(configuration_name);
  configuration_item->SetFileName(file_name);

  // appending configuration to the container
  (void)container.InsertItem(std::move(configuration_item), mvvm::TagIndex::Append());
}

}  // namespace sup::gui
