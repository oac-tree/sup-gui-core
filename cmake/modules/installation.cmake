# -----------------------------------------------------------------------------
# Installation
# Credits to https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right,
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/Exporting-and-Importing-Targets
# -----------------------------------------------------------------------------

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/sup-gui-core)

# exporting targets to a script and installing it
install(EXPORT sup-gui-core-targets FILE sup-gui-core-targets.cmake NAMESPACE sup-gui-core:: DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Exporting
# -----------------------------------------------------------------------------

# Add all targets to the build-tree export set
export(TARGETS sup-gui-core sup-gui-tools NAMESPACE sup-gui-core:: FILE "${PROJECT_BINARY_DIR}/sup-gui-core-targets.cmake")

# Export the package for use from the build-tree (goes to $HOME/.cmake)
if(COA_EXPORT_BUILD_TREE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE sup-gui-core)
endif()

# -----------------------------------------------------------------------------
# Creating and installing sup-gui-core-config.cmake
# -----------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

# to use in the build tree
configure_package_config_file(${SUP_GUI_CORE_PROJECT_DIR}/cmake/configs/sup-gui-core-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sup-gui-core-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# to use in install tree
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-gui-core-config.cmake DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Create and install sup-gui-core-config-version.cmake file
# -----------------------------------------------------------------------------

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sup-gui-core-config-version.cmake VERSION
    ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-gui-core-config-version.cmake DESTINATION ${INSTALL_CONFIGDIR})
