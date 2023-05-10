// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#ifndef EXPERIMENTAL_TASK_H
#define EXPERIMENTAL_TASK_H

/**
 * @brief The ITask class represents a task to do.
 *
 * @note It is expected that the task is self-contained, that it can run in a thread safely and that
 * it stores the result of the calculation on board.
 *
 * @note The task is intended to be executed via the Worker class, which will prohibit an access to
 * the task during its execution.
 */

class ITask
{
public:
  virtual ~ITask() = default;

  /**
   * @brief Runs calculation
   */
  virtual void Run() = 0;
};

class ReadConfigTask : public ITask
{
public:
};

#endif  // EXPERIMENTAL_TASK_H
