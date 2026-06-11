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

#include "sup/gui/views/dtoeditor/splittable_editor_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/views/dtoeditor/config_tab_widget.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>

#include <gtest/gtest.h>

#include <QSplitter>

namespace sup::gui::test
{

class SplittableEditorControllerTest : public ::testing::Test
{
public:
  /**
   * @brief Create test controller.
   */
  std::unique_ptr<SplittableEditorController> CreateController()
  {
    auto factory_func = []() -> std::unique_ptr<SplittableComboPanel>
    {
      auto main_editor = std::make_unique<ConfigTabWidget>();
      return std::make_unique<SplittableComboPanel>(std::move(main_editor));
    };

    return std::make_unique<SplittableEditorController>(&m_splitter, factory_func);
  }

  QSplitter m_splitter;
};

TEST_F(SplittableEditorControllerTest, InitialState)
{
  // no splitter defined
  EXPECT_THROW(const SplittableEditorController controller(nullptr, {}), RuntimeException);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetFocusWidget(), nullptr);
}

TEST_F(SplittableEditorControllerTest, CreatePanel)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  EXPECT_EQ(controller->GetFocusWidget(), panel1);
  EXPECT_EQ(m_splitter.count(), 1);

  auto* panel2 = controller->CreatePanel(panel1);
  EXPECT_EQ(controller->GetFocusWidget(), panel2);
  EXPECT_EQ(m_splitter.count(), 2);

  // panel between two panels
  auto* panel3 = controller->CreatePanel(panel1);
  EXPECT_EQ(controller->GetFocusWidget(), panel3);
  EXPECT_EQ(m_splitter.count(), 3);

  EXPECT_EQ(m_splitter.widget(0), panel1);
  EXPECT_EQ(m_splitter.widget(1), panel3);
  EXPECT_EQ(m_splitter.widget(2), panel2);
}

TEST_F(SplittableEditorControllerTest, CreateSinglePanelAndThenClose)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel1);
  controller->ClosePanel(panel1);

  EXPECT_EQ(controller->GetFocusWidget(), nullptr);
  EXPECT_EQ(m_splitter.count(), 1);  // because deletion is deferred

  // but the widget itself is already hidden
  EXPECT_FALSE(panel1->isVisible());
}

TEST_F(SplittableEditorControllerTest, CreateThreePanelsAndThenCloseTheMiddle)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel(panel1);
  auto* panel3 = controller->CreatePanel(panel1);

  EXPECT_EQ(controller->GetFocusWidget(), panel3);
  controller->ClosePanel(panel2);
  EXPECT_EQ(controller->GetFocusWidget(), panel3);
  EXPECT_EQ(m_splitter.count(), 3);  // because deletion is deferred
  EXPECT_FALSE(panel2->isVisible());
}

TEST_F(SplittableEditorControllerTest, CreateThreePanelsAndThenCloseTheFocused)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel(panel1);
  auto* panel3 = controller->CreatePanel(panel1);

  EXPECT_EQ(controller->GetFocusWidget(), panel3);
  controller->ClosePanel(panel3);
  EXPECT_EQ(controller->GetFocusWidget(), panel2);  // focus should move to the previous widget
  EXPECT_EQ(m_splitter.count(), 3);                 // because deletion is deferred
  EXPECT_FALSE(panel3->isVisible());
}

TEST_F(SplittableEditorControllerTest, CreatePanelAndThenCreatePanelsViaWidgetSignals)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel1);

  panel1->splitViewRequest();  // should create panel2 after panel1
  EXPECT_EQ(m_splitter.count(), 2);
  auto* panel2 = qobject_cast<SplittableComboPanel*>(m_splitter.widget(1));
  EXPECT_EQ(controller->GetFocusWidget(), panel2);
}

TEST_F(SplittableEditorControllerTest, CreateThreePanelsAndThenCloseFirstOneViaWidgetSignal)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel(panel1);
  auto* panel3 = controller->CreatePanel(panel1);

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  panel1->closeViewRequest();        // should close panel1
  EXPECT_EQ(m_splitter.count(), 3);  // because deletion is deferred
  EXPECT_FALSE(panel1->isVisible());

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  // closing focused panel
  panel3->closeViewRequest();        // should close panel3
  EXPECT_EQ(m_splitter.count(), 3);  // because deletion is deferred
  EXPECT_FALSE(panel3->isVisible());
  EXPECT_EQ(controller->GetFocusWidget(), panel2);  // focus should move to the previous widget
}

}  // namespace sup::gui::test
