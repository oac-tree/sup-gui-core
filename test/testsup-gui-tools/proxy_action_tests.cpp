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

#include "sup/gui/widgets/proxy_action.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sup::gui;

class ProxyActionTest : public ::testing::Test
{
};

TEST_F(ProxyActionTest, InitialState)
{
  ProxyAction action;
  EXPECT_TRUE(action.isEnabled());
}

//! Validating how SetAction propagates properties of real action to the proxy.

TEST_F(ProxyActionTest, SetAction)
{
  const QString expected_name("abc");
  const QString expected_tooltip("tooltip");

  QAction real_action(expected_name);
  real_action.setToolTip(expected_tooltip);

  ProxyAction proxy_action;
  proxy_action.SetAction(&real_action);

  EXPECT_EQ(proxy_action.GetAction(), &real_action);
  EXPECT_EQ(proxy_action.text(), expected_name);
  EXPECT_EQ(proxy_action.toolTip(), expected_tooltip);
  EXPECT_TRUE(proxy_action.isEnabled());
}

//! Validating how triggering proxy propagates to real action.

TEST_F(ProxyActionTest, TriggerProxy)
{
  const QString expected_name("abc");
  const QString expected_tooltip("tooltip");

  QAction real_action(expected_name);

  ProxyAction proxy_action;
  QSignalSpy spy_real_action(&real_action, &ProxyAction::triggered);

  proxy_action.SetAction(&real_action);
  EXPECT_EQ(proxy_action.text(), expected_name);

  // triggering proxy
  proxy_action.trigger();

  // real action should be triggered too
  EXPECT_EQ(spy_real_action.count(), 1);

  // resetting proxy
  proxy_action.SetAction(nullptr);
  EXPECT_EQ(proxy_action.text(), QString("Proxy"));  // defined in proxy_action.cpp

  // triggering proxy
  proxy_action.trigger();

  // real action wasn't triggered anymore
  EXPECT_EQ(spy_real_action.count(), 1);
}

//! Triggering real action shouldn't be propagated to the proxy.

TEST_F(ProxyActionTest, TriggerRealAction)
{
  const QString expected_name("abc");

  QAction real_action(expected_name);

  ProxyAction proxy_action;
  QSignalSpy spy_real_action(&real_action, &ProxyAction::triggered);
  QSignalSpy spy_proxy_action(&proxy_action, &ProxyAction::triggered);

  proxy_action.SetAction(&real_action);
  EXPECT_EQ(proxy_action.text(), expected_name);

  // triggering real action
  real_action.trigger();

  // real action should be triggered too
  EXPECT_EQ(spy_real_action.count(), 1);
  EXPECT_EQ(spy_proxy_action.count(), 0);
}

//! Check how enabled disabled status is propagated to the proxy object.

TEST_F(ProxyActionTest, SetEnabled)
{
  const QString expected_name("abc");

  QAction real_action(expected_name);

  ProxyAction proxy_action;

  proxy_action.SetAction(&real_action, ProxyAction::Options::SyncEnabledStatus);
  EXPECT_EQ(proxy_action.text(), expected_name);

  // initial status
  EXPECT_TRUE(real_action.isEnabled());
  EXPECT_TRUE(proxy_action.isEnabled());

  // disabling real action should lead to proxy disabling
  real_action.setEnabled(false);
  EXPECT_FALSE(real_action.isEnabled());
  EXPECT_FALSE(proxy_action.isEnabled());

  // enabling real action should lead to proxy enabling
  real_action.setEnabled(true);
  EXPECT_TRUE(real_action.isEnabled());
  EXPECT_TRUE(proxy_action.isEnabled());
}

//! Do not track enabled/disabled status.

TEST_F(ProxyActionTest, DoNotTrackEnabled)
{
  const QString expected_name("abc");

  QAction real_action(expected_name);

  ProxyAction proxy_action;

  proxy_action.SetAction(&real_action, ProxyAction::Options::None);
  EXPECT_EQ(proxy_action.text(), expected_name);

  // initial status
  EXPECT_TRUE(real_action.isEnabled());
  EXPECT_TRUE(proxy_action.isEnabled());

  // disabling real action should lead to proxy disabling
  real_action.setEnabled(false);
  EXPECT_FALSE(real_action.isEnabled());
  EXPECT_TRUE(proxy_action.isEnabled());
}
