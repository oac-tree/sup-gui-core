set(test testsup-gui-benchmark)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE benchmark::benchmark sup-gui-views sup-gui-core-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
