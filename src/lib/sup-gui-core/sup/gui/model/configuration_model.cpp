
#include "configuration_model.h"

#include "configuration_container_item.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/model_utils.h>

namespace sup::gui
{

ConfigurationModel::ConfigurationModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("ConfigurationModel", std::move(pool))

{
  PopulateModel();
}

void ConfigurationModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

ConfigurationContainerItem* ConfigurationModel::GetConfigurationContainer()
{
  return m_configuration_container;
}

void ConfigurationModel::PopulateModel()
{
  m_configuration_container = InsertItem<ConfigurationContainerItem>();
  if (auto command_stack = mvvm::ApplicationModel::GetCommandStack(); command_stack)
  {
    command_stack->Clear();  // to disallow "undo" of our main container
  }
}

}  // namespace sup::gui
