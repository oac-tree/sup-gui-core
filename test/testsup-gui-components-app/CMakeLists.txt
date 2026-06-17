# Unit tests for sup-gui-components library (which require QApplication)

set(test testsup-gui-components-app)

add_executable(${test})

target_sources(${test} PRIVATE
  action_map_tests.cpp
  app_command_context_widget_map_tests.cpp
  app_command_registry_tests.cpp
  app_command_service_tests.cpp
  app_command_tests.cpp
  app_context_focus_controller_tests.cpp
  custom_row_waveform_strategies_tests.cpp
  dto_composer_tab_controller_tests.cpp
  main.cpp
  menu_action_container_tests.cpp
  menu_bar_action_registry_tests.cpp
  proxy_action_tests.cpp
  scroll_area_mirror_controller_tests.cpp
  tab_widget_controller_tests.cpp
  tree_expand_controller_tests.cpp
  tree_helper_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-gui-components sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST)
