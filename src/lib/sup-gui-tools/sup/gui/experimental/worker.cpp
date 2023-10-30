/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "worker.h"

#include "task.h"

Worker::Worker(std::unique_ptr<ITask> task) : m_task(std::move(task)), m_status(kIdle) {}

Worker::~Worker() = default;

void Worker::Run()
{
  auto worker_func = [this]()
  {
    try
    {
      SetStatus(kStarted);
      m_task->Run();
      SetStatus(kCompleted);
    }
    catch (...)
    {
      SetStatus(kFailed);
    }
  };

  m_future = std::async(std::launch::async, worker_func);
}

std::unique_ptr<ITask> Worker::WaitForResult()
{
  m_future.wait();
  return std::move(m_task);
}

Worker::Status Worker::GetStatus() const
{
  return m_status.load();
}

void Worker::SetStatus(Status value)
{
  // atomic might not be enough, lock may be?
  m_status.store(value);
  emit StatusChanged(value);
}
