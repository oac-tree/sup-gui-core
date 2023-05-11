// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

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
  auto watcher = worker.GetFutureWatcher();
  EXPECT_TRUE(watcher->isStarted());
  EXPECT_FALSE(watcher->isRunning());
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
  auto watcher = worker.GetFutureWatcher();

  QSignalSpy spy_started(watcher, &QFutureWatcher<void>::started);
  QSignalSpy spy_finished(watcher, &QFutureWatcher<void>::finished);

  EXPECT_CALL(mock_task, Run()).Times(1);

  // triggering expectation
  worker.Run();

  QApplication::processEvents();

  QTest::qWait(50);

  watcher->waitForFinished();

  EXPECT_EQ(spy_started.count(), 1);
  EXPECT_EQ(spy_finished.count(), 1);

  EXPECT_FALSE(watcher->isRunning());
  auto result = worker.MoveResult();

  EXPECT_EQ(result.get(), task_ptr);
}

// TEST_F(WorkerManagerTest, WorkerManagerStart)
//{
//   qRegisterMetaType<Worker*>("Worker*");

//  QStringListModel model;
//  auto task = std::make_unique<AddLineTask>("text", model);
//  auto task_ptr = task.get();

//  WorkerManager manager;
//  QSignalSpy spy_worker_started(&manager, &WorkerManager::WorkerStarted);
//  QSignalSpy spy_worker_finished(&manager, &WorkerManager::WorkerStarted);

//  manager.Start(std::move(task));

//  EXPECT_EQ(manager.GetWorkerCount(), 1);
//  QTest::qWait(50);

//  EXPECT_EQ(spy_worker_started.count(), 1);
//  EXPECT_EQ(spy_worker_finished.count(), 1);
//}
