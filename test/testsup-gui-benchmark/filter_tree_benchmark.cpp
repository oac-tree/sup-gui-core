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

//! Profiling of the AnyValueEditorTreePanel filtering field.

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/viewmodel/anyvalue_viewmodel.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <sup/dto/anyvalue.h>

#include <QSortFilterProxyModel>
#include <QTreeView>
#include <benchmark/benchmark.h>
#include <testutils/cmake_info.h>

#include <cstdlib>
#include <functional>
#include <memory>

namespace sup::gui::test
{

namespace
{

const int kIterations = 3;

/**
 * @brief Returns the JSON file to profile, overridable via SUP_BENCH_JSON.
 */
std::string GetTestJsonFileName()
{
  if (const char* env = std::getenv("SUP_BENCH_JSON"); env != nullptr)
  {
    return env;
  }
  return ProjectResourceDir() + "/sdd/sdd-fast.json";
}

const sup::dto::AnyValue& GetCachedAnyValue()
{
  static const sup::dto::AnyValue result = AnyValueFromJSONFile(GetTestJsonFileName());
  return result;
}

/**
 * @brief Counting version of the production filter proxy, to see how many rows are visited.
 */
class CountingFilterViewModel : public mvvm::FilterNameViewModel
{
public:
  bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override
  {
    ++m_calls;
    return mvvm::FilterNameViewModel::filterAcceptsRow(source_row, source_parent);
  }

  mutable std::size_t m_calls{0};
};

/**
 * @brief Holds the model plus viewmodel/proxy/view chain, as TreeViewComponentProvider does.
 */
struct TreeFixture
{
  std::unique_ptr<mvvm::ApplicationModel> model;
  std::unique_ptr<QTreeView> view;
  std::unique_ptr<mvvm::ViewModel> viewmodel;
  std::unique_ptr<CountingFilterViewModel> proxy;

  explicit TreeFixture(bool recursive_filtering = true, bool show_view = false)
      : model(std::make_unique<mvvm::ApplicationModel>()), view(std::make_unique<QTreeView>())
  {
    model->InsertItem(CreateAnyValueItem(GetCachedAnyValue()), model->GetRootItem(),
                      mvvm::TagIndex::Append());

    viewmodel = std::make_unique<AnyValueViewModel>(model.get());
    proxy = std::make_unique<CountingFilterViewModel>();
    proxy->setRecursiveFilteringEnabled(recursive_filtering);
    proxy->setSourceModel(viewmodel.get());
    view->setModel(proxy.get());
    if (show_view)
    {
      view->show();
    }
  }

  /**
   * @brief Brings the tree back to the state the benchmark wants to measure from.
   *
   * Collapsing before clearing the pattern keeps the restore cheap: with few expanded nodes the
   * proxy's row insertions have almost no persistent indexes to walk.
   */
  void Restore(bool expand_all)
  {
    view->collapseAll();
    proxy->SetPattern(QString{});
    if (expand_all)
    {
      view->expandAll();
    }
    proxy->m_calls = 0;
  }
};

}  // namespace

class FilterBenchmark : public benchmark::Fixture
{
public:
  FilterBenchmark()
  {
    Unit(benchmark::kMillisecond);
    Iterations(kIterations);
  }
};

//! How big the tree actually is.
BENCHMARK_F(FilterBenchmark, Filter00_TreeSize)(benchmark::State& state)
{
  for (auto dummy : state)
  {
    const auto item = CreateAnyValueItem(GetCachedAnyValue());
    state.counters["anyvalue_items"] =
        static_cast<double>(mvvm::utils::GetItemCount<AnyValueItem>(*item));
  }
}

//! The first keystroke with the tree collapsed.
BENCHMARK_F(FilterBenchmark, Filter01_FirstKeystroke_Collapsed)(benchmark::State& state)
{
  TreeFixture fixture;

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.Restore(/*expand_all*/ false);
    state.ResumeTiming();

    fixture.proxy->SetPattern("a");

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.counters["proxy_root_rows"] = fixture.proxy->rowCount(QModelIndex());
    state.ResumeTiming();
  }
}

//! Subsequent keystrokes, i.e. what the user perceives as "responsive later".
BENCHMARK_F(FilterBenchmark, Filter02_LaterKeystrokes_Collapsed)(benchmark::State& state)
{
  TreeFixture fixture;
  fixture.proxy->SetPattern("a");
  const QStringList patterns{"ab", "abc", "abcd"};
  int index{0};

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.proxy->m_calls = 0;
    const auto pattern = patterns.at(index % patterns.size());
    ++index;
    state.ResumeTiming();

    fixture.proxy->SetPattern(pattern);

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.ResumeTiming();
  }
}

//! The first keystroke with the tree fully expanded, as ImportAnyValueFromFile leaves it.
BENCHMARK_F(FilterBenchmark, Filter03_FirstKeystroke_Expanded)(benchmark::State& state)
{
  TreeFixture fixture;

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.Restore(/*expand_all*/ true);
    state.ResumeTiming();

    fixture.proxy->SetPattern("a");

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.ResumeTiming();
  }
}

//! Same as Filter03, but with the view actually shown, to check that visibility is irrelevant.
BENCHMARK_F(FilterBenchmark, Filter03b_FirstKeystroke_Expanded_ViewShown)(benchmark::State& state)
{
  TreeFixture fixture(/*recursive_filtering*/ true, /*show_view*/ true);

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.Restore(/*expand_all*/ true);
    state.ResumeTiming();

    fixture.proxy->SetPattern("a");

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.ResumeTiming();
  }
}

//! Expanded tree, but with the view detached, i.e. with no persistent indexes on the proxy.
//! Isolates the proxy's own filtering cost from the persistent-index bookkeeping.
BENCHMARK_F(FilterBenchmark, Filter07_FirstKeystroke_Expanded_ViewDetached)
(benchmark::State& state)
{
  TreeFixture fixture;

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.view->setModel(fixture.proxy.get());
    fixture.Restore(/*expand_all*/ true);
    fixture.view->setModel(nullptr);
    state.ResumeTiming();

    fixture.proxy->SetPattern("a");

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.ResumeTiming();
  }
}

//! Mitigation A: collapse the tree before changing the pattern, then let the view re-expand.
BENCHMARK_F(FilterBenchmark, Filter08_FirstKeystroke_CollapseAllFirst)(benchmark::State& state)
{
  TreeFixture fixture;

  for (auto dummy : state)
  {
    state.PauseTiming();
    fixture.Restore(/*expand_all*/ true);
    state.ResumeTiming();

    fixture.view->collapseAll();
    fixture.proxy->SetPattern("a");
    fixture.view->expandAll();

    state.PauseTiming();
    state.counters["accepts_row_calls"] = static_cast<double>(fixture.proxy->m_calls);
    state.counters["proxy_root_rows"] = fixture.proxy->rowCount(QModelIndex());
    state.ResumeTiming();
  }
}

//! Cost of the underlying data() call the filter does, for scale.
BENCHMARK_F(FilterBenchmark, Filter06_RawDataAccessOverWholeTree)(benchmark::State& state)
{
  TreeFixture fixture;
  auto* source = fixture.viewmodel.get();

  for (auto dummy : state)
  {
    std::size_t visited{0};
    std::function<void(const QModelIndex&)> walk = [&](const QModelIndex& parent)
    {
      const int rows = source->rowCount(parent);
      for (int row = 0; row < rows; ++row)
      {
        const auto index = source->index(row, 0, parent);
        benchmark::DoNotOptimize(
            source->data(index, Qt::DisplayRole).toString().toLower().contains("a"));
        ++visited;
        walk(index);
      }
    };
    walk(QModelIndex());
    state.counters["visited"] = static_cast<double>(visited);
  }
}

}  // namespace sup::gui::test
