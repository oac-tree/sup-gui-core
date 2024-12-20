/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
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

#include "sup/gui/widgets/custom_splitter_controller.h"

#include <sup/gui/core/exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QDebug>
#include <QSplitter>
#include <QTest>

namespace sup::gui
{

/**
 * @brief Testing CustomSplitterController class.
 */
class CustomSplitterControllerTest : public ::testing::Test
{
public:
  ::testing::MockFunction<read_variant_func_t> m_mock_read_func;
  ::testing::MockFunction<write_variant_func_t> m_mock_write_func;
};

TEST_F(CustomSplitterControllerTest, InitialState)
{
  EXPECT_THROW(CustomSplitterController("abc", nullptr), RuntimeException);
  QSplitter splitter;

  EXPECT_THROW(CustomSplitterController("", &splitter), RuntimeException);

  CustomSplitterController controller1("group1", &splitter);
  EXPECT_FALSE(controller1.GetMainStateKey().isEmpty());
  EXPECT_FALSE(controller1.GetChildrenStateKey().isEmpty());

  CustomSplitterController controller2("group2", &splitter);
  EXPECT_NE(controller1.GetMainStateKey(), controller2.GetMainStateKey());
  EXPECT_NE(controller1.GetChildrenStateKey(), controller2.GetChildrenStateKey());
}

TEST_F(CustomSplitterControllerTest, WriteSettingsOfEmptySplitter)
{
  QSplitter splitter;
  const QString group_name("abc");

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  const auto expected_splitter_state = QVariant::fromValue(splitter.saveState());
  const auto expected_setting_name = group_name + "_state";  // in custom_splitter_controller.cpp
  EXPECT_CALL(m_mock_write_func, Call(expected_setting_name, expected_splitter_state)).Times(1);

  CustomSplitterController controller(group_name, &splitter);
  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(CustomSplitterControllerTest, WriteSettingsSplitterWithTwoWidgets)
{
  auto widget1 = new QWidget;
  auto widget2 = new QWidget;

  QSplitter splitter;
  const QString group_name("abc");

  splitter.addWidget(widget1);
  splitter.addWidget(widget2);

  splitter.show();
  EXPECT_TRUE(widget1->isVisible());
  EXPECT_TRUE(widget2->isVisible());

  CustomSplitterController controller(group_name, &splitter);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);

  const auto expected_splitter_state = QVariant::fromValue(splitter.saveState());
  // two flags denoting widget visibility
  QVariant expected_children_state = QVariant::fromValue(QList<int>() << 1 << 1);

  EXPECT_CALL(m_mock_write_func, Call(controller.GetMainStateKey(), expected_splitter_state))
      .Times(1);
  EXPECT_CALL(m_mock_write_func, Call(controller.GetChildrenStateKey(), expected_children_state))
      .Times(1);

  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(CustomSplitterControllerTest, WriteSettingsSplitterWithTwoWidgetsWhenOneIsHidden)
{
  auto widget1 = new QWidget;
  auto widget2 = new QWidget;

  QSplitter splitter;
  const QString group_name("abc");

  splitter.addWidget(widget1);
  splitter.addWidget(widget2);

  splitter.show();

  widget2->setVisible(false);

  EXPECT_TRUE(widget1->isVisible());
  EXPECT_FALSE(widget2->isVisible());

  CustomSplitterController controller(group_name, &splitter);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);

  const auto expected_splitter_state = QVariant::fromValue(splitter.saveState());
  // two flags denoting widget visibility
  QVariant expected_children_state = QVariant::fromValue(QList<int>() << 1 << 0);

  EXPECT_CALL(m_mock_write_func, Call(controller.GetMainStateKey(), expected_splitter_state))
      .Times(1);
  EXPECT_CALL(m_mock_write_func, Call(controller.GetChildrenStateKey(), expected_children_state))
      .Times(1);

  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

}  // namespace sup::gui
