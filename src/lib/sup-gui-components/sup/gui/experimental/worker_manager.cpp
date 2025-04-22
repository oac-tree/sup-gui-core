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
  (void)connect(worker_it.get(), &Worker::StatusChanged, this, worker_status_changed,
                Qt::QueuedConnection);

  worker_it->Run();

  return worker_it.get();
}

std::size_t WorkerManager::GetWorkerCount() const
{
  return m_workers.size();
}

std::unique_ptr<ITask> WorkerManager::TakeResult(Worker* worker)
{
  auto result = worker->WaitForResult();

  // removes worker
  auto index = mvvm::utils::IndexOfItem(m_workers, worker);
  (void)m_workers.erase(std::next(m_workers.begin(), index));

  // returns completed task
  return result;
}
