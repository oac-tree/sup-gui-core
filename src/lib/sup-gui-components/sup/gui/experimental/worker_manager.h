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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_EXPERIMENTAL_WORKER_MANAGER_H_
#define SUP_GUI_EXPERIMENTAL_WORKER_MANAGER_H_

#include <QObject>
#include <cstddef>
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

  std::size_t GetWorkerCount() const;

  /**
   * @brief Takes the result corresponding to a given worker. Removes worker from a queue.
   */
  std::unique_ptr<ITask> TakeResult(Worker* worker);

signals:
  void WorkerStatusChanged(Worker*, std::size_t status);

private:
  std::vector<std::unique_ptr<Worker>> m_workers;
};

#endif  // SUP_GUI_EXPERIMENTAL_WORKER_MANAGER_H_
