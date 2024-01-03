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

#include "sup/gui/experimental/worker_manager.h"

#include "sup/gui/experimental/task.h"
#include "sup/gui/experimental/worker.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QFutureWatcher>
#include <QSignalSpy>
#include <QString>
#include <QStringListModel>
#include <QTest>
#include <thread>

using ::testing::_;

/**
 * @brief The AddLineTask represent a task to add a string line to a model.
 */
class AddLineTask : public ITask
{
public:
  AddLineTask(const QString& pattern, QStringListModel& model) : m_pattern(pattern), m_model(model)
  {
  }

  //! Heavy calculations
  void Run() override { m_data << m_pattern; }

  //! Should be called in GUI thread to finally update the model
  void Finalize() override { m_model.setStringList(m_data); }

private:
  QStringList m_data;         //!< buffer to populate in the course of heavy Run
  QStringList m_pattern;      //!< initial prerequisites
  QStringListModel& m_model;  //!<
};

//! To test ITask with gmock framework.
class MockTask : public ITask
{
public:
  MOCK_METHOD(void, Run, ());
};

//! Decorator for ITask. Used to wrap MockTask and pass it inside Worker.
class TaskDecorator : public ITask
{
public:
  TaskDecorator(ITask* component) : m_component(component) {}
  void Run() override { m_component->Run(); }
  ITask* m_component;
};

class WorkerManagerTest : public ::testing::Test
{
public:
  using msec = std::chrono::milliseconds;
};

TEST_F(WorkerManagerTest, WorkerInitialState)
{
  Worker worker(std::make_unique<MockTask>());
  EXPECT_EQ(worker.GetStatus(), Worker::kIdle);
}

TEST_F(WorkerManagerTest, WorkerRun)
{
  // creating mocking task to control Run method call
  MockTask mock_task;

  // Wrapping mock task inside decorator to pass inside the Worker.
  // This we need because it is not possible to use gmock framework if mock_task lives the scope.
  auto task = std::make_unique<TaskDecorator>(&mock_task);
  auto task_ptr = task.get();

  Worker worker(std::move(task));

  EXPECT_CALL(mock_task, Run()).Times(1);

  // triggering expectation
  worker.Run();

  QTest::qWait(50);

  EXPECT_EQ(worker.GetStatus(), Worker::kCompleted);
  auto result = worker.WaitForResult();

  EXPECT_EQ(result.get(), task_ptr);
}

TEST_F(WorkerManagerTest, WorkerManagerStart)
{
  qRegisterMetaType<Worker*>("Worker*");

  // creating a model and task to modify this model
  QStringListModel model;
  auto task = std::make_unique<AddLineTask>("text", model);
  auto task_ptr = task.get();

  EXPECT_EQ(model.rowCount(QModelIndex()), 0);

  // create a manager for given task and start the worker
  WorkerManager manager;
  QSignalSpy spy_worker(&manager, &WorkerManager::WorkerStatusChanged);

  auto worker = manager.Start(std::move(task));

  EXPECT_EQ(manager.GetWorkerCount(), 1);
  QTest::qWait(50);

  // at the end of work
  EXPECT_EQ(spy_worker.count(), 2);
  EXPECT_EQ(worker->GetStatus(), Worker::kCompleted);

  // taking the result
  auto result = manager.TakeResult(worker);
  EXPECT_EQ(result.get(), task_ptr);
  EXPECT_EQ(manager.GetWorkerCount(), 0);

  // model is still empty since this is how we have designed our task
  EXPECT_EQ(model.rowCount(QModelIndex()), 0);

  // finalizing the task
  result->Finalize();

  // the model should be populated
  EXPECT_EQ(model.rowCount(QModelIndex()), 1);
}
