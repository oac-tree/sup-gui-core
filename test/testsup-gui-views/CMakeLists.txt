set(test testsup-gui-views)

add_executable(${test})

target_sources(${test} PRIVATE
  anyvalue_editor_app_context_tests.cpp
  anyvalue_editor_tests.cpp
  anyvalue_waveform_editor_tests.cpp
  combo_tool_bar_tests.cpp
  configuration_list_widget_tests.cpp
  configuration_panel_mirror_controller_tests.cpp
  configuration_splittable_widget_tests.cpp
  configuration_tab_widget_tests.cpp
  container_helper_tests.cpp
  custom_splitter_controller_tests.cpp
  dto_compare_view_tests.cpp
  dto_composer_view_tests.cpp
  dto_explorer_view_tests.cpp
  item_display_name_combo_adapter_tests.cpp
  item_stack_widget_tests.cpp
  main.cpp
  settings_helper_tests.cpp
  settings_splitter_tests.cpp
  splittable_combo_panel_tests.cpp
  splittable_editor_controller_tests.cpp
  stacked_widget_provider_tests.cpp
  tree_view_component_provider_tests.cpp
  waveform_editor_tests.cpp
  waveform_table_component_provider_tests.cpp
  waveform_table_widget_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-gui-views sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST)
