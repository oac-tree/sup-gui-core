// ----------------------------------------------------------------------------
// Prototype of worker/task machinery.
// ----------------------------------------------------------------------------

#include "sup/gui/experimental/worker_manager.h"

#include "sup/gui/experimental/task.h"
#include "sup/gui/experimental/worker.h"

#include <gtest/gtest.h>

#include <QString>
#include <QStringListModel>

/**
 * @brief The AddLineTask represent a task to add a string line to a model.
 */
class AddLineTask : public ITask
{
public:
  AddLineTask(const QString& pattern, QStringListModel& model) : m_pattern(pattern), m_model(model)
  {
  }

  //! Heavy calculations
  void Run() override { m_data << m_pattern; }

  //! Should be called in GUI thread to finally update the model
  void Finalize() override { m_model.setStringList(m_data); }

private:
  QStringList m_data;         //!< buffer to populate in the course of heavy Run
  QStringList m_pattern;      //!< initial prerequisites
  QStringListModel& m_model;  //!<
};

class WorkerManagerTest : public ::testing::Test
{
public:
};

TEST_F(WorkerManagerTest, Run)
{
  QStringListModel model;

  auto task = std::make_unique<AddLineTask>("text", model);

  WorkerManager manager;
  manager.Start(std::move(task));
}
