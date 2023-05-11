// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "worker_manager.h"

#include "task.h"
#include "worker.h"

#include <mvvm/utils/container_utils.h>

WorkerManager::~WorkerManager() = default;

Worker* WorkerManager::Start(std::unique_ptr<ITask> task)
{
  auto& worker_it = m_workers.emplace_back(std::make_unique<Worker>(std::move(task)));

  auto worker_status_changed = [this, &worker_it](int status)
  { emit WorkerStatusChanged(worker_it.get(), status); };
  connect(worker_it.get(), &Worker::StatusChanged, this, worker_status_changed,
          Qt::QueuedConnection);

  worker_it->Run();

  return worker_it.get();
}

int WorkerManager::GetWorkerCount() const
{
  return static_cast<int>(m_workers.size());
}

std::unique_ptr<ITask> WorkerManager::TakeResult(Worker* worker)
{
  auto result = worker->WaitForResult();

  // removes worker
  auto index = mvvm::utils::IndexOfItem(m_workers, worker);
  m_workers.erase(std::next(m_workers.begin(), index));

  // returns completed task
  return std::move(result);
}
