// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#ifndef EXPERIMENTAL_WORKER_MANAGER_H
#define EXPERIMENTAL_WORKER_MANAGER_H

#include <QObject>
#include <map>
#include <memory>

class Worker;
class ITask;

/**
 * @brief The WorkerManager class manages the running of tasks.
 *
 * @note Internally it creates a collection of workers
 */

class WorkerManager : public QObject
{
  Q_OBJECT

public:
  ~WorkerManager() override;

  Worker* Start(std::unique_ptr<ITask> task);

  int GetWorkerCount() const;

  /**
   * @brief Takes the result corresponding to a given worker. Removes worker from a queue.
   */
  std::unique_ptr<ITask> TakeResult(Worker* worker);

signals:
  void WorkerStatusChanged(Worker*, int status);

private:
  std::vector<std::unique_ptr<Worker>> m_workers;
};

#endif  // EXPERIMENTAL_WORKER_MANAGER_H
