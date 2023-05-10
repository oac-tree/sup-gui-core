// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "worker_manager.h"

#include "worker.h"
#include "task.h"

#include <QFutureWatcher>

void WorkerManager::Start(std::unique_ptr<ITask> task)
{
  auto worker = std::make_unique<Worker>(std::move(task));
  auto result = worker.get();

  auto watcher = worker->GetFutureWatcher();
  connect(watcher, );

}

int WorkerManager::GetWorkerCount() const
{
  return static_cast<int>(m_workers.size());
}
