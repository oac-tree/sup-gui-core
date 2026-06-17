set(test testsup-gui-benchmark)

add_executable(${test})

target_sources(${test} PRIVATE
  main.cpp
  transform_large_anyvalue_benchmark.cpp
)

target_link_libraries(${test} PRIVATE benchmark::benchmark sup-mvvm::test sup-gui-views sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
