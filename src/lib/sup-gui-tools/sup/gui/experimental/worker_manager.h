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
 * @brief The WorkerManager class manages creation and running of the workers. It also provide a way
 * to be notified
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

signals:
  void StatusChanged(Worker*);

private:
  std::vector<std::unique_ptr<Worker>> m_workers;
};

#endif  // EXPERIMENTAL_WORKER_MANAGER_H
