/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_EXPERIMENTAL_WORKER_H_
#define SUP_GUI_EXPERIMENTAL_WORKER_H_

#include <QObject>
#include <atomic>
#include <future>
#include <memory>

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
  ~Worker() override;

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

#endif  // SUP_GUI_EXPERIMENTAL_WORKER_H_
