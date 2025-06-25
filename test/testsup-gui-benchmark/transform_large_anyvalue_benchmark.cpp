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

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <sup/dto/anyvalue.h>

#include <benchmark/benchmark.h>
#include <testutils/cmake_info.h>

namespace sup::gui::test
{

/**
 * @brief Testing performance of functions to convert AnyValue to AnyValueItem and back.
 */
class TransformLargeAnyValueBenchmark : public benchmark::Fixture
{
public:
  TransformLargeAnyValueBenchmark() { Unit(benchmark::kMillisecond); }
  /**
   * @brief Returns a path to havy JSON file containing AnyValue representation.
   */
  static std::string GetTestJsonString()
  {
    return ProjectResourceDir() + "/anyvalue-editor/cis-configuration.json";
  }
};

BENCHMARK_F(TransformLargeAnyValueBenchmark, AnyValueFromJSONString)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());

  for (auto dummy : state)
  {
    const auto anyvalue = AnyValueFromJSONString(json_content);
  }
}

BENCHMARK_F(TransformLargeAnyValueBenchmark, CreateAnyValueItem)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());
  const auto anyvalue = AnyValueFromJSONString(json_content);

  for (auto dummy : state)
  {
    auto item = CreateAnyValueItem(anyvalue);
  }
}

BENCHMARK_F(TransformLargeAnyValueBenchmark, InsertAnyValueItem)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());
  const auto anyvalue = AnyValueFromJSONString(json_content);

  for (auto dummy : state)
  {
    mvvm::ApplicationModel model;
    auto item = CreateAnyValueItem(anyvalue);
    model.InsertItem(std::move(item), model.GetRootItem(), mvvm::TagIndex::Append());
  }
}

BENCHMARK_F(TransformLargeAnyValueBenchmark,
            InsertAnyValueItemWhenViewModel)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());
  const auto anyvalue = AnyValueFromJSONString(json_content);

  for (auto dummy : state)
  {
    mvvm::ApplicationModel model;
    mvvm::AllItemsViewModel viewmodel(&model);
    auto item = CreateAnyValueItem(anyvalue);
    model.InsertItem(std::move(item), model.GetRootItem(), mvvm::TagIndex::Append());
  }
}

BENCHMARK_F(TransformLargeAnyValueBenchmark, ExportItemToAnyValue)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());
  const auto anyvalue = AnyValueFromJSONString(json_content);
  const auto item = CreateAnyValueItem(anyvalue);

  for (auto dummy : state)
  {
    auto anyvalue = CreateAnyValue(*item);
  }
}

BENCHMARK_F(TransformLargeAnyValueBenchmark, AnyValueToJSONString)(benchmark::State& state)
{
  const std::string json_content = mvvm::test::GetTextFileContent(GetTestJsonString());
  const auto anyvalue = AnyValueFromJSONString(json_content);

  for (auto dummy : state)
  {
    const std::string json_string = AnyTypeToJSONString(anyvalue);
  }
}

}  // namespace sup::gui::test
