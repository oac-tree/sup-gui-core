// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#ifndef EXPERIMENTAL_TASK_H
#define EXPERIMENTAL_TASK_H

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
   * @brief Runs main calculations.
   */
  virtual void Run() = 0;

  /**
   * @brief Convenience method that can be called in a GUI thread to finalize the task.
   * For example, it can perform updates of GUI model with the data obtained on previous step.
   */
  virtual void Finalize(){};
};

#endif  // EXPERIMENTAL_TASK_H
