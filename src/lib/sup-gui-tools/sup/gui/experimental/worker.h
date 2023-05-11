// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#ifndef EXPERIMENTAL_WORKER_H
#define EXPERIMENTAL_WORKER_H

#include <QFuture>
#include <QFutureWatcher>
#include <memory>

class ITask;

/**
 * @brief The Worker class is intended to execute a task in a thread.
 *
 * @note The worker is initialised with the task and takes ownership of it. It is expected
 * that the task is self contained and can run in a thread safely with no data race. When the task
 * is complete, the worker will allow to take the task back.
 *
 * @note Worker provide an access to future watcher, and it's up to the user to subscibe to
 * necessary signals. It's also user's responsibility to manage time-of-life of the Worker.
 *
 * @note Please note, that the QFuture(void) is used here only to wait for result is completed.
 * The result itself is stored on board of the task.
 *
 * @note No inheritance on Qt classes. Class is not expected to be derived from.
 */

class Worker
{
public:
  using watcher_t = QFutureWatcher<void>;
  using future_t = QFuture<void>;

  explicit Worker(std::unique_ptr<ITask> task);
  ~Worker() = default;

  /**
   * @brief Runs a task in a thread.
   */
  void Run();

  std::unique_ptr<ITask> MoveResult();

  std::unique_ptr<ITask> WaitForResult();

  watcher_t* GetFutureWatcher();

private:
  future_t m_future;
  std::unique_ptr<watcher_t> m_future_watcher;
  std::unique_ptr<ITask> m_task;
};

#endif  // EXPERIMENTAL_WORKER_H
