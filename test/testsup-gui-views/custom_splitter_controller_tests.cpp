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

#include "sup/gui/widgets/custom_splitter_controller.h"

#include <sup/gui/components/custom_metatypes.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/mainwindow/main_window_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSplitter>
#include <QTest>
#include <cstdlib>

namespace sup::gui::test
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

//! Checking that Qt5 setup can handle QList<int>
TEST_F(CustomSplitterControllerTest, VariantForList)
{
  QList<int> list{{1, 2, 3}};

  auto variant1 = QVariant::fromValue(list);
  auto variant2 = QVariant::fromValue(list);
  EXPECT_EQ(variant1, variant2);

  auto list_from_variant = variant1.value<QList<int>>();
  EXPECT_EQ(list, list_from_variant);
}

TEST_F(CustomSplitterControllerTest, InitialState)
{
  EXPECT_THROW(CustomSplitterController("abc", nullptr), RuntimeException);
  QSplitter splitter;

  EXPECT_NO_THROW(CustomSplitterController("", &splitter));

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

  CustomSplitterController controller(group_name, &splitter);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  const auto expected_splitter_state = QVariant::fromValue(splitter.saveState());
  EXPECT_CALL(m_mock_write_func, Call(controller.GetMainStateKey(), expected_splitter_state))
      .Times(1);

  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(CustomSplitterControllerTest, AttemptToWriteSettingsWhenNoGroupIsDefined)
{
  QSplitter splitter;
  const QString group_name("");

  CustomSplitterController controller(group_name, &splitter);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_write_func, Call(::testing::_, ::testing::_)).Times(0);

  EXPECT_THROW(controller.WriteSettings(m_mock_write_func.AsStdFunction()), RuntimeException);
}

TEST_F(CustomSplitterControllerTest, WriteSettingsSplitterWithTwoWidgets)
{
  if (IsHeadlessMode())
  {
    GTEST_SKIP();
  }

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
  QVariant expected_children_state = QVariant::fromValue(QList<int>({1, 1}));

  EXPECT_CALL(m_mock_write_func, Call(controller.GetMainStateKey(), expected_splitter_state))
      .Times(1);
  EXPECT_CALL(m_mock_write_func, Call(controller.GetChildrenStateKey(), expected_children_state))
      .Times(1);

  controller.UpdateChildrenVisibilityCache();
  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(CustomSplitterControllerTest, WriteSettingsSplitterWithTwoWidgetsWhenOneIsHidden)
{
  if (IsHeadlessMode())
  {
    GTEST_SKIP();
  }

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
  QVariant expected_children_state = QVariant::fromValue(QList<int>({1, 0}));

  EXPECT_CALL(m_mock_write_func, Call(controller.GetMainStateKey(), expected_splitter_state))
      .Times(1);
  EXPECT_CALL(m_mock_write_func, Call(controller.GetChildrenStateKey(), expected_children_state))
      .Times(1);

  controller.UpdateChildrenVisibilityCache();
  controller.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(CustomSplitterControllerTest, ReadSettingsOfEmptySplitter)
{
  QSplitter splitter;
  const QString group_name("abc");

  EXPECT_CALL(m_mock_write_func, Call(::testing::_, ::testing::_)).Times(0);

  CustomSplitterController controller(group_name, &splitter);

  const auto expected_splitter_state = QVariant::fromValue(splitter.saveState());
  EXPECT_CALL(m_mock_read_func, Call(controller.GetMainStateKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(controller.GetChildrenStateKey())).Times(1);

  controller.ReadSettings(m_mock_read_func.AsStdFunction());

  // mimick situation when number of flags doesn't match widget count
  QList<int> flags({42});
  ON_CALL(m_mock_read_func, Call(controller.GetChildrenStateKey()))
      .WillByDefault(::testing::Return(QVariant::fromValue(flags)));

  EXPECT_CALL(m_mock_read_func, Call(controller.GetMainStateKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(controller.GetChildrenStateKey())).Times(1);

  EXPECT_NO_THROW(controller.ReadSettings(m_mock_read_func.AsStdFunction()));
}

TEST_F(CustomSplitterControllerTest, AttemptToReadSettingsWhenNoGroupIsDefined)
{
  QSplitter splitter;
  const QString group_name("");

  EXPECT_CALL(m_mock_write_func, Call(::testing::_, ::testing::_)).Times(0);

  CustomSplitterController controller(group_name, &splitter);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);

  EXPECT_THROW(controller.ReadSettings(m_mock_read_func.AsStdFunction()), RuntimeException);
}

//! We are validating settings restore mechanism. We can't check exact panel sizes of the splitter
//! after setting restore because of the splitter nature. But we can check that small panels become
//! large and vice versa.
TEST_F(CustomSplitterControllerTest, ReadSettingsSplitterWithTwoWidgets)
{
  if (IsHeadlessMode())
  {
    GTEST_SKIP();
  }

  QByteArray splitter_state;

  // preparing splitter to use its state
  {
    auto widget1 = new QWidget;
    auto widget2 = new QWidget;

    QSplitter splitter;
    const QString group_name("abc");

    splitter.addWidget(widget1);
    splitter.addWidget(widget2);

    splitter.show();

    splitter.setSizes({20, 40});

    EXPECT_TRUE(widget1->isVisible());
    EXPECT_TRUE(widget2->isVisible());

    auto current_sizes = splitter.sizes();
    EXPECT_TRUE(current_sizes.at(0) < current_sizes.at(1));
    splitter_state = splitter.saveState();
  }

  // new splitter
  auto widget1 = new QWidget;
  auto widget2 = new QWidget;

  QSplitter splitter;
  const QString group_name("abc");

  splitter.addWidget(widget1);
  splitter.addWidget(widget2);

  splitter.show();
  splitter.setSizes({40, 20});

  auto initial_sizes = splitter.sizes();
  EXPECT_TRUE(initial_sizes.at(0) > initial_sizes.at(1));

  // restoring new splitter using the state of old splitter
  CustomSplitterController controller(group_name, &splitter);

  QList<int> flags({1, 0});  // pretending second widget is hidden

  // setting up callbacks so they returns widget settings and splitter state
  ON_CALL(m_mock_read_func, Call(controller.GetMainStateKey()))
      .WillByDefault(::testing::Return(QVariant::fromValue(splitter_state)));
  ON_CALL(m_mock_read_func, Call(controller.GetChildrenStateKey()))
      .WillByDefault(::testing::Return(QVariant::fromValue(flags)));

  EXPECT_CALL(m_mock_read_func, Call(controller.GetMainStateKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(controller.GetChildrenStateKey())).Times(1);

  controller.ReadSettings(m_mock_read_func.AsStdFunction());

  // Sizes of new splitter changes.
  auto new_sizes = splitter.sizes();
  EXPECT_TRUE(new_sizes.at(0) < new_sizes.at(1));
  EXPECT_NE(initial_sizes, new_sizes);

  EXPECT_TRUE(widget1->isVisible());
  EXPECT_FALSE(widget2->isVisible());
}

}  // namespace sup::gui::test
