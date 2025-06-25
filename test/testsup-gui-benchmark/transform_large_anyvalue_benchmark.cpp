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

#include <benchmark/benchmark.h>

#include <sup/gui/model/anyvalue_utils.h>

namespace sup::gui::test
{

/**
 * @brief Testing performance of functions to convert AnyValue to AnyValueItem and back.
 */
class TransformLargeAnyValueBenchmark : public benchmark::Fixture
{
public:
};

BENCHMARK_F(TransformLargeAnyValueBenchmark, AnyValueFromJSONString)(benchmark::State& state)
{
  for (auto dummy : state)
  {
  }
}

}  // namespace sup::gui::test
