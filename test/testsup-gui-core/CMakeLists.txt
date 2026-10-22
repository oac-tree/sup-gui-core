set(test testsup-gui-core)

add_executable(${test})

target_sources(${test} PRIVATE
  anyvalue_conversion_utils_tests.cpp
  anyvalue_item_builder_tests.cpp
  anyvalue_item_tests.cpp
  anyvalue_item_undo_tests.cpp
  anyvalue_item_utils_tests.cpp
  anyvalue_utils_tests.cpp
  configuration_container_item_tests.cpp
  configuration_item_tests.cpp
  configuration_model_tests.cpp
  domain_anyvalue_builder_tests.cpp
  import_helper_tests.cpp
  main.cpp
  message_event_tests.cpp
  message_handler_decorator_tests.cpp
  persistent_settings_model_tests.cpp
  query_result_tests.cpp
  scalar_conversion_utils_tests.cpp
  settings_container_item_tests.cpp
  standard_message_handlers_tests.cpp
  sup_dto_model_tests.cpp
  sup_gui_core_exceptions_tests.cpp
  undo_settings_item_tests.cpp
  version_helper_tests.cpp
  version_tests.cpp
  waveform_model_tests.cpp
)

target_link_libraries(${test} PRIVATE sup-gui-core sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST)
