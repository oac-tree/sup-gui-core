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
 * @brief The WorkerManager class manages creation and running of the workers. It also allows to
 * subscribe to notifications
 *
 * @note It is not intended
 */

class WorkerManager : public QObject
{
  Q_OBJECT

public:
  ~WorkerManager() override;

  void Start(std::unique_ptr<ITask> task);

  int GetWorkerCount() const;

  std::unique_ptr<ITask> TakeResult(Worker* worker);

signals:
  void WorkerStarted(Worker*);
  void WorkerFinished(Worker*);

private:
  std::vector<std::unique_ptr<Worker>> m_workers;
};

#endif  // EXPERIMENTAL_WORKER_MANAGER_H
