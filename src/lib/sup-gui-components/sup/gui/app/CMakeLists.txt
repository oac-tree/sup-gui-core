# Collection of helper functions and classes related to the whole application.

target_sources(${library_name} PRIVATE
  app_action_helper.cpp
  app_action_helper.h
  app_action_manager.cpp
  app_action_manager.h
  app_command.cpp
  app_command.h
  app_command_manager.cpp
  app_command_manager.h
  app_constants.h
  app_context.cpp
  app_context.h
  app_context_focus_controller.cpp
  app_context_focus_controller.h
  app_context_manager.cpp
  app_context_manager.h
  app_helper.cpp
  app_helper.h
  main_window_types.h
  )
