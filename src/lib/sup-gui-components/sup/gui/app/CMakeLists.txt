# Collection of helper functions and classes related to the whole application.

target_sources(${library_name} PRIVATE
  app_command.cpp
  app_command.h
  app_command_context.cpp
  app_command_context.h
  app_command_context_widget_map.cpp
  app_command_context_widget_map.h
  app_command_registry.cpp
  app_command_registry.h
  app_command_service.cpp
  app_command_service.h
  app_constants.h
  app_context_focus_controller.cpp
  app_context_focus_controller.h
  app_helper.cpp
  app_helper.h
  default_command_service.cpp
  default_command_service.h
  i_action_container.h
  i_action_container_registry.h
  i_app_command_context_widget_map.h
  i_app_command_registry.h
  i_app_command_service.h
  menu_action_container.cpp
  menu_action_container.h
  menu_bar_action_registry.cpp
  menu_bar_action_registry.h
  null_command_service.cpp
  null_command_service.h
  )
