if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sup-mvvm
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sup-gui-core-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
  src-sup-gui-core=${CMAKE_SOURCE_DIR}/src/lib/sup-gui-core
  src-sup-gui-components=${CMAKE_SOURCE_DIR}/src/lib/sup-gui-components
  src-sup-gui-views=${CMAKE_SOURCE_DIR}/src/lib/sup-gui-views
  TARGETS sup-gui-core sup-gui-components sup-gui-views
)

get_target_property(SUP_GUI_VIEWS_INCLUDE_DIRECTORIES sup-gui-views INCLUDE_DIRECTORIES)
target_include_directories(sup-gui-core-cpptest PUBLIC ${SUP_GUI_VIEWS_INCLUDE_DIRECTORIES})

target_link_libraries(sup-gui-core-cpptest
  PUBLIC
  sup-mvvm::view
  sup-gui-extra::ksyntaxhighlighting
  sup-dto::sup-dto
)
