// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#ifndef EXPERIMENTAL_WORKER_H
#define EXPERIMENTAL_WORKER_H

#include <QObject>
#include <atomic>
#include <memory>
#include <future>

class ITask;

/**
 * @brief The Worker class is intended to execute a task in a thread.
 *
 * @note The worker is initialised with the task and takes ownership of it. It is expected
 * that the task is self contained and can run in a thread safely with no data race. When the task
 * is complete, the worker allows to take the task back.
 * *
 * @note Please note, that the QFuture(void) is used here only to wait for result is completed.
 * The result itself is stored on board of the task.
 *
 * @note Worker has signals that has to be connected via queued connection vith the GUI thread.
 * Worker is supposed to be used via WorkerManager.
 *
 * @note Current design doesn't use QFutureWatcher machinery to report the progress. I understood
 * that it has some pecularities in Qt5, since QPromise was introduced in Qt6 only.
 */

class Worker : public QObject
{
  Q_OBJECT

public:
  enum Status
  {
    kIdle,
    kStarted,
    kCompleted,
    kFailed
  };

  explicit Worker(std::unique_ptr<ITask> task);
  ~Worker();

  /**
   * @brief Runs a task in a thread.
   */
  void Run();

  std::unique_ptr<ITask> WaitForResult();

  Status GetStatus() const;
  void SetStatus(Status value);

signals:
  void StatusChanged(int);

private:
  std::future<void> m_future;
  std::unique_ptr<ITask> m_task;
  std::atomic<Status> m_status;
};

#endif  // EXPERIMENTAL_WORKER_H
