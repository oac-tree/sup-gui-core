// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "worker.h"

#include "task.h"

#include <QtConcurrent>
#include <iostream>

Worker::Worker(std::unique_ptr<ITask> task)
    : m_future_watcher(std::make_unique<watcher_t>()), m_task(std::move(task))
{
}

void Worker::Run()
{
  if (m_future_watcher->isRunning())
  {
    throw std::runtime_error("Computation was started already");
  }

  auto worker_func = [this]()
  {
    m_task->Run();
  };

  m_future_watcher->setFuture(QtConcurrent::run(worker_func));
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
