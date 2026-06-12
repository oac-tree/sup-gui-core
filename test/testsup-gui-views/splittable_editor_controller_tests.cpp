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
#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/config_tab_widget.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>

#include <mvvm/model/session_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QSplitter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

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

      auto item_list_callback = []() -> std::vector<mvvm::SessionItem*> { return {}; };

      return std::make_unique<SplittableComboPanel>(std::move(main_editor), item_list_callback);
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
  EXPECT_EQ(m_splitter.count(), 0);
  EXPECT_TRUE(controller->GetPanels().empty());
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

  std::vector<SplittableComboPanel*> expected_panels;
  expected_panels.push_back(qobject_cast<SplittableComboPanel*>(m_splitter.widget(0)));
  expected_panels.push_back(qobject_cast<SplittableComboPanel*>(m_splitter.widget(1)));
  expected_panels.push_back(qobject_cast<SplittableComboPanel*>(m_splitter.widget(2)));

  EXPECT_EQ(controller->GetPanels(), expected_panels);
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

  std::vector<SplittableComboPanel*> expected_panels;
  expected_panels.push_back(qobject_cast<SplittableComboPanel*>(m_splitter.widget(0)));
  expected_panels.push_back(qobject_cast<SplittableComboPanel*>(m_splitter.widget(1)));
  EXPECT_EQ(controller->GetPanels(), expected_panels);
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

  panel1->splitPanelRequest();  // should create panel2 after panel1
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

  panel1->closePanelRequest();       // should close panel1
  EXPECT_EQ(m_splitter.count(), 3);  // because deletion is deferred
  EXPECT_FALSE(panel1->isVisible());

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  // closing focused panel
  panel3->closePanelRequest();       // should close panel3
  EXPECT_EQ(m_splitter.count(), 3);  // because deletion is deferred
  EXPECT_FALSE(panel3->isVisible());
  EXPECT_EQ(controller->GetFocusWidget(), panel2);  // focus should move to the previous widget
}

TEST_F(SplittableEditorControllerTest, SetFocusProgrammatically)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel();
  auto* panel3 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  controller->SetFocusWidget(panel1);
  EXPECT_EQ(controller->GetFocusWidget(), panel1);
}

TEST_F(SplittableEditorControllerTest, SetFocusViaFocusRequest)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel();
  auto* panel3 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  panel1->focusPanelRequest();  // should set focus to panel1
  EXPECT_EQ(controller->GetFocusWidget(), panel1);
}

TEST_F(SplittableEditorControllerTest, FocusPanelEmitsSelectionRequest)
{
  const mvvm::SessionItem item1;
  const mvvm::ItemSelection selection1(&item1);
  const mvvm::SessionItem item2;
  const mvvm::ItemSelection selection2(&item2);

  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel();
  auto* panel3 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  QSignalSpy spy_selection_changed(controller.get(),
                                   &SplittableEditorController::focusPanelSelectionChanged);

  // focus panel emit request
  panel3->itemSelectionChanged(selection1);
  EXPECT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(spy_selection_changed), selection1);

  // emitting the same selection should not trigger the signal
  panel3->itemSelectionChanged(selection1);
  EXPECT_EQ(spy_selection_changed.count(), 0);

  // selecting another one
  panel3->itemSelectionChanged(selection2);
  EXPECT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(spy_selection_changed), selection2);
}

TEST_F(SplittableEditorControllerTest, NonFocusPanelEmitsSelectionRequest)
{
  const mvvm::SessionItem item1;
  const mvvm::ItemSelection selection1(&item1);

  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel();
  auto* panel3 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  const QSignalSpy spy_selection_changed(controller.get(),
                                         &SplittableEditorController::focusPanelSelectionChanged);

  // non-focused panel emit request, controller should be silent
  panel2->itemSelectionChanged(selection1);
  EXPECT_EQ(spy_selection_changed.count(), 0);
}

TEST_F(SplittableEditorControllerTest, SetModel)
{
  auto controller = CreateController();

  auto* panel1 = controller->CreatePanel();
  auto* panel2 = controller->CreatePanel();
  auto* panel3 = controller->CreatePanel();

  EXPECT_EQ(controller->GetFocusWidget(), panel3);

  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  controller->SetModel(&model);
  EXPECT_EQ(panel1->GetModel(), &model);
  EXPECT_EQ(panel2->GetModel(), &model);
  EXPECT_EQ(panel3->GetModel(), &model);
}

}  // namespace sup::gui::test
