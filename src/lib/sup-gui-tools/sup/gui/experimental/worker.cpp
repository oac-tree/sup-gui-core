// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "worker.h"

#include "task.h"

#include <QtConcurrent>
#include <iostream>
#include <thread>
#include <QThreadPool>

Worker::Worker(std::unique_ptr<ITask> task)
    : m_future_watcher(std::make_unique<watcher_t>()), m_task(std::move(task))
{
}

void Worker::Run()
{
  // The biggest problem here is that Qt5 doesn't have a QPromise yet.
  // Also it seems that QtConcurrent::run doesn't allow to use QFutureWatcher directly.
  // This all leads to akward future/watcher/pool interplay, imho.

  if (m_future_watcher->isRunning())
  {
    throw std::runtime_error("Computation was started already");
  }

  //  auto worker_func = [this]() { m_task->Run(); };
  std::function<void(void)> spin = [this]()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    m_task->Run();
  };

  auto pool = QThreadPool::globalInstance();

  //  m_future = std::make_unique<future_t>(QtConcurrent::run(spin));
  //  m_future = QtConcurrent::run(spin);
  //  m_future_watcher->setFuture(m_future);
}

std::unique_ptr<ITask> Worker::MoveResult()
{
  if (!m_future_watcher->future().isFinished())
  {
    throw std::runtime_error("Data is not yet ready");
  }

  return std::move(m_task);
}

std::unique_ptr<ITask> Worker::WaitForResult()
{
  m_future_watcher->waitForFinished();
  return std::move(m_task);
}

Worker::watcher_t *Worker::GetFutureWatcher()
{
  return m_future_watcher.get();
}
