// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "worker_manager.h"

#include "task.h"
#include "worker.h"

#include <QFutureWatcher>

WorkerManager::~WorkerManager() = default;

void WorkerManager::Start(std::unique_ptr<ITask> task)
{
  //  auto worker = std::make_unique<Worker>(std::move(task));
  //  auto worker_ptr = worker.get();

  //  auto watcher = worker->GetFutureWatcher();

  //  auto worker_started = [this, worker_ptr]() { emit WorkerStarted(worker_ptr); };
  //  connect(watcher, &QFutureWatcher<void>::started, this, worker_started);

  //  auto worker_finished = [this, worker_ptr]() { emit WorkerStarted(worker_ptr); };
  //  connect(watcher, &QFutureWatcher<void>::finished, this, worker_finished);

  //  m_workers.push_back(std::move(worker));

  //  worker_ptr->Run();

  auto& worker_it = m_workers.emplace_back(std::make_unique<Worker>(std::move(task)));
  auto watcher = worker_it->GetFutureWatcher();

  auto worker_started = [this, &worker_it]() { emit WorkerStarted(worker_it.get()); };
  connect(watcher, &QFutureWatcher<void>::started, this, worker_started);
  auto worker_finished = [this, &worker_it]() { emit WorkerFinished(worker_it.get()); };
  connect(watcher, &QFutureWatcher<void>::started, this, worker_finished);

  worker_it->Run();
}

int WorkerManager::GetWorkerCount() const
{
  return static_cast<int>(m_workers.size());
}

std::unique_ptr<ITask> WorkerManager::TakeResult(Worker* worker)
{
  //  if (index >= 0 && index < GetItemCount())
  //  {
  //      auto item = std::move(m_items.at(index));
  //      m_items.erase(std::next(m_items.begin(), index));
  //      return std::move(item);
  //  }

  return {};
}
