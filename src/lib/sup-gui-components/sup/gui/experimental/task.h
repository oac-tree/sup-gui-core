/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_EXPERIMENTAL_TASK_H_
#define SUP_GUI_EXPERIMENTAL_TASK_H_

/**
 * @brief The ITask class represents a task to do.
 *
 * @note It is expected that the task is self-contained, that it can run safely in a thread and that
 * it stores the result of the calculation on board.
 *
 * @note The task is intended to be executed via the Worker class, which will prohibit an access to
 * the task during its execution.
 *
 * @note No inheritance on Qt classes.
 */

class ITask
{
public:
  virtual ~ITask() = default;

  /**
   * @brief Runs main calculations. Expected to be called from non-GUI thread.
   */
  virtual void Run() = 0;

  /**
   * @brief Convenience method that can be called in a GUI thread to finalize the task.
   * For example, it can perform updates of GUI model with the data obtained on previous step.
   */
  virtual void Finalize() {};
};

#endif  // SUP_GUI_EXPERIMENTAL_TASK_H_
