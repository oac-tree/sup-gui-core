# Unit tests for sup-gui-components library

set(test testsup-gui-components)

add_executable(${test})

target_sources(${test} PRIVATE
  abstract_text_content_controller_tests.cpp
  anyvalue_editor_action_folder_tests.cpp
  anyvalue_editor_action_handler_copypaste_tests.cpp
  anyvalue_editor_action_handler_extended_tests.cpp
  anyvalue_editor_action_handler_tests.cpp
  anyvalue_editor_action_handler_undoredo_tests.cpp
  anyvalue_editor_helper_tests.cpp
  anyvalue_editor_project_tests.cpp
  anyvalue_item_copy_helper_tests.cpp
  anyvalue_item_extented_tests.cpp
  anyvalue_viewmodel_tests.cpp
  app_command_context_tests.cpp
  app_helper_tests.cpp
  app_settings_storage_tests.cpp
  custom_children_waveform_strategies_tests.cpp
  custom_row_strategies_tests.cpp
  dto_composer_action_handler_tests.cpp
  dto_editor_project_tests.cpp
  dto_waveform_action_handler_tests.cpp
  in_memory_settings_storage_tests.cpp
  item_filter_helper_tests.cpp
  json_panel_controller_tests.cpp
  main.cpp
  mime_conversion_helper_tests.cpp
  null_command_service_tests.cpp
  scoped_callback_tests.cpp
  settings_archive_tests.cpp
  settings_manager_tests.cpp
  waveform_display_controller_tests.cpp
  waveform_editor_action_hadler_tests.cpp
  waveform_helper_tests.cpp
  waveform_twocolumn_viewmodel_tests.cpp
  widget_focus_handler_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-gui-components sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST)
