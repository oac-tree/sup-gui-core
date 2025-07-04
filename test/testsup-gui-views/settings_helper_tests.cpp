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

#include "sup/gui/mainwindow/settings_helper.h"

#include <sup/gui/model/settings_model.h>

#include <mvvm/model/session_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for methods from settings_helper.h
 */
class SettingsHelperTest : public ::testing::Test
{
public:
  ::testing::MockFunction<read_variant_func_t> m_mock_read_func;
  ::testing::MockFunction<write_variant_func_t> m_mock_write_func;
};

TEST_F(SettingsHelperTest, WriteAndReadEmptyModel)
{
  mvvm::SessionModel model;

  EXPECT_CALL(m_mock_write_func, Call(::testing::_, ::testing::_)).Times(0);
  WriteSettingsToPersistentStorage(model, m_mock_write_func.AsStdFunction());

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction());
}

}  // namespace sup::gui::test
