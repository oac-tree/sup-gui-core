set(library_name sup-gui-core)
set(export_name core)

add_library(${library_name} SHARED "")
add_library(sup-gui-core::${export_name} ALIAS ${library_name}) # alias for build-tree usage

set_target_properties(${library_name} PROPERTIES
  EXPORT_NAME ${export_name}
  VERSION ${LIBVERSION}
  SOVERSION ${LIBSOVERSION}
)

target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::model sup-dto::sup-dto)

add_subdirectory(sup)

target_include_directories(${library_name} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SUP_GUI_CORE_AUTOGEN_DIR}>)

install(TARGETS ${library_name} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS ${library_name} EXPORT sup-gui-core-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sup/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup FILES_MATCHING PATTERN "*.h")
