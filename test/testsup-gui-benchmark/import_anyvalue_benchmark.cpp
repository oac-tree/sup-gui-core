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

//! Profiling of the AnyValueEditorWidget::ImportAnyValueFromFile pipeline. Every benchmark here
//! reproduces one stage of the real import, in the same configuration as the running application:
//! an ApplicationModel with undo enabled, an AnyValueViewModel behind a QTreeView, and a
//! JsonPanelController regenerating the JSON view. Run with --benchmark_filter=Import.

#include <sup/gui/components/json_panel_controller.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/views/anyvalueeditor/tree_view_component_provider.h>
#include <sup/gui/views/codeeditor/code_view.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <sup/dto/anyvalue.h>

#include <QPlainTextEdit>
#include <QTreeView>
#include <benchmark/benchmark.h>
#include <testutils/cmake_info.h>

#include <cstdlib>
#include <memory>

namespace sup::gui::test
{

namespace
{

const bool kEnableUndo = true;
const std::size_t kUndoLimit = 100;

/**
 * @brief Returns the JSON file to profile, overridable via SUP_BENCH_JSON.
 */
std::string GetTestJsonFileName()
{
  if (const char* env = std::getenv("SUP_BENCH_JSON"); env != nullptr)
  {
    return env;
  }
  return ProjectResourceDir() + "/sdd/sdd.json";
}

/**
 * @brief Returns AnyValue parsed from the test file, parsed only once for the whole run.
 */
const sup::dto::AnyValue& GetCachedAnyValue()
{
  static const sup::dto::AnyValue result = AnyValueFromJSONFile(GetTestJsonFileName());
  return result;
}

/**
 * @brief Returns the total number of items in a tree, including the given one.
 */
int CountItems(const mvvm::SessionItem& item)
{
  int result = 1;
  for (auto child : item.GetAllItems())
  {
    result += CountItems(*child);
  }
  return result;
}

}  // namespace

class ImportBenchmark : public benchmark::Fixture
{
public:
  ImportBenchmark() { Unit(benchmark::kMillisecond); }
};

//! Stage 1: reading the file and parsing JSON into AnyValue.
BENCHMARK_F(ImportBenchmark, Import01_ParseJsonFile)(benchmark::State& state)
{
  for (auto dummy : state)
  {
    const auto anyvalue = AnyValueFromJSONFile(GetTestJsonFileName());
  }
}

//! Stage 2: building the AnyValueItem tree out of the AnyValue.
BENCHMARK_F(ImportBenchmark, Import02_CreateAnyValueItem)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    auto item = CreateAnyValueItem(anyvalue);
    state.counters["items"] = CountItems(*item);
  }
}

//! Stage 3a: inserting into a bare model, no undo, no observers. The floor cost.
BENCHMARK_F(ImportBenchmark, Import03a_InsertBareModel)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());

      state.PauseTiming();
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 3b: same insertion, but with undo enabled as the application does.
BENCHMARK_F(ImportBenchmark, Import03b_InsertWithUndo)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      model->SetUndoEnabled(kEnableUndo, kUndoLimit);
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());

      state.PauseTiming();
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 4a: insertion while the tree view model is already listening, as in the application.
BENCHMARK_F(ImportBenchmark, Import04a_InsertWithTreeViewModelAttached)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      QTreeView tree_view;
      TreeViewComponentProvider provider(nullptr, &tree_view);
      provider.SetItem(model->GetRootItem());
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());

      state.PauseTiming();
      provider.SetItem(nullptr);
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 4b: the same result reached by attaching the view model only after insertion.
BENCHMARK_F(ImportBenchmark, Import04b_InsertThenAttachTreeViewModel)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      QTreeView tree_view;
      TreeViewComponentProvider provider(nullptr, &tree_view);
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());
      provider.SetItem(model->GetRootItem());

      state.PauseTiming();
      provider.SetItem(nullptr);
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 5a: insertion while the JSON panel controller is listening, as in the application.
BENCHMARK_F(ImportBenchmark, Import05a_InsertWithJsonPanelAttached)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      int update_count{0};
      auto on_text = [&update_count](const std::string&) { ++update_count; };
      auto on_message = [](const MessageEvent&) {};
      auto controller =
          std::make_unique<JsonPanelController>(model->GetRootItem(), on_text, on_message);
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());

      state.PauseTiming();
      state.counters["json_updates"] = update_count;
      controller.reset();
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 5b: the same result reached by attaching the JSON panel only after insertion.
BENCHMARK_F(ImportBenchmark, Import05b_InsertThenAttachJsonPanel)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      auto on_text = [](const std::string&) {};
      auto on_message = [](const MessageEvent&) {};
      auto item = CreateAnyValueItem(anyvalue);
      state.ResumeTiming();

      model->InsertItem(std::move(item), model->GetRootItem(), mvvm::TagIndex::Append());
      auto controller =
          std::make_unique<JsonPanelController>(model->GetRootItem(), on_text, on_message);

      state.PauseTiming();
      controller.reset();
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! Stage 5c: destroying a fully populated model, which the import also pays when replacing data.
BENCHMARK_F(ImportBenchmark, Import05c_DestroyPopulatedModel)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    auto model = std::make_unique<mvvm::ApplicationModel>();
    model->InsertItem(CreateAnyValueItem(anyvalue), model->GetRootItem(),
                      mvvm::TagIndex::Append());
    state.ResumeTiming();

    model.reset();
  }
}

//! Stage 5d: generating the JSON text the right panel displays (AnyValueItem -> AnyValue -> JSON).
BENCHMARK_F(ImportBenchmark, Import05d_GenerateJsonText)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();
  const auto item = CreateAnyValueItem(anyvalue);

  for (auto dummy : state)
  {
    auto exported = CreateAnyValue(*item);
    const std::string text = AnyValueToJSONString(exported, /*pretty*/ true);
    state.counters["json_kbytes"] = static_cast<double>(text.size()) / 1024.0;
  }
}

//! Stage 5e: pushing that JSON text into the syntax-highlighting view, as the text panel does.
BENCHMARK_F(ImportBenchmark, Import05e_JsonViewSetContent)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();
  const auto item = CreateAnyValueItem(anyvalue);
  auto exported = CreateAnyValue(*item);
  const auto text = QString::fromStdString(AnyValueToJSONString(exported, /*pretty*/ true));

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      CodeView view(CodeView::kJSON);
      view.show();
      state.ResumeTiming();

      view.SetContent(text);

      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

//! Stage 5f: the same text into a plain text edit, i.e. without the syntax highlighter, to
//! separate the cost of the highlighting from the cost of laying out 28 MB of text.
BENCHMARK_F(ImportBenchmark, Import05f_PlainTextEditSetContent)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();
  const auto item = CreateAnyValueItem(anyvalue);
  auto exported = CreateAnyValue(*item);
  const auto text = QString::fromStdString(AnyValueToJSONString(exported, /*pretty*/ true));

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      QPlainTextEdit view;
      view.setReadOnly(true);
      view.show();
      state.ResumeTiming();

      view.setPlainText(text);

      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

//! Stage 6: QTreeView::expandAll() over the fully populated tree.
BENCHMARK_F(ImportBenchmark, Import06_ExpandAll)(benchmark::State& state)
{
  const auto& anyvalue = GetCachedAnyValue();

  for (auto dummy : state)
  {
    state.PauseTiming();
    {
      auto model = std::make_unique<mvvm::ApplicationModel>();
      QTreeView tree_view;
      TreeViewComponentProvider provider(nullptr, &tree_view);
      provider.SetItem(model->GetRootItem());
      model->InsertItem(CreateAnyValueItem(anyvalue), model->GetRootItem(),
                        mvvm::TagIndex::Append());
      tree_view.show();
      state.ResumeTiming();

      tree_view.expandAll();

      state.PauseTiming();
      provider.SetItem(nullptr);
      model.reset();
    }
    state.ResumeTiming();
  }
}

//! The whole pipeline, assembled exactly as the application does it.
BENCHMARK_F(ImportBenchmark, Import07_FullPipelineAsInApplication)(benchmark::State& state)
{
  for (auto dummy : state)
  {
    state.PauseTiming();
    mvvm::ApplicationModel model;
    model.SetUndoEnabled(kEnableUndo, kUndoLimit);
    QTreeView tree_view;
    TreeViewComponentProvider provider(nullptr, &tree_view);
    auto on_text = [](const std::string&) {};
    auto on_message = [](const MessageEvent&) {};
    JsonPanelController controller(model.GetRootItem(), on_text, on_message);
    provider.SetItem(model.GetRootItem());
    tree_view.show();
    state.ResumeTiming();

    auto anyvalue = AnyValueFromJSONFile(GetTestJsonFileName());
    auto item = CreateAnyValueItem(anyvalue);
    model.InsertItem(std::move(item), model.GetRootItem(), mvvm::TagIndex::Append());
    tree_view.expandAll();
  }
}

}  // namespace sup::gui::test
