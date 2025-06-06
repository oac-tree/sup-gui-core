# -----------------------------------------------------------------------------
# Installation
# Credits to https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right,
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/Exporting-and-Importing-Targets
# -----------------------------------------------------------------------------

include(PackageConfig)
include(CMakePackageConfigHelpers)

set(BUILD_CONFIGDIR ${CMAKE_CURRENT_BINARY_DIR})
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/sup-gui-core)

# -----------------------------------------------------------------------------
# Exporting targets
# -----------------------------------------------------------------------------
set(PACKAGE_TARGETS_FILENAME sup-gui-core-targets.cmake)
set(PACKAGE_TARGETS_FILE ${BUILD_CONFIGDIR}/${PACKAGE_TARGETS_FILENAME})

# Install the export targets for installation usage, this does not relate to the build tree file
install(EXPORT sup-gui-core-targets FILE ${PACKAGE_TARGETS_FILENAME} NAMESPACE sup-gui-core:: DESTINATION ${INSTALL_CONFIGDIR})

# Generate the export targets for the build tree usage
export(TARGETS sup-gui-core sup-gui-components sup-gui-views NAMESPACE sup-gui-core:: FILE ${PACKAGE_TARGETS_FILE})

# Export the package to CMake registry for build tree usage (goes to $HOME/.cmake)
if(COA_EXPORT_BUILD_TREE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE sup-gui-core)
endif()

# -----------------------------------------------------------------------------
# Version configuration
# -----------------------------------------------------------------------------
set(PACKAGE_VERSION_FILE ${BUILD_CONFIGDIR}/sup-gui-core-config-version.cmake)

# Generate the version config file, shared in both build tree and installation usage
write_basic_package_version_file(${PACKAGE_VERSION_FILE} COMPATIBILITY AnyNewerVersion)

install(FILES ${PACKAGE_VERSION_FILE} DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Package configuration
# -----------------------------------------------------------------------------
set(PACKAGE_CONFIG_FILE ${BUILD_CONFIGDIR}/sup-gui-core-config.cmake)

if(CODAC_FOUND)
  # Set NO_CMAKE_PATH and PATHS to CMAKE_PREFIX_PATH, so that find_package will use the system Qt first
  # if it finds one, but still look in CMAKE_PREFIX_PATH as a last resort. This gives system Qt priority over CODAC Qt
  set(QT_FIND_OPTIONS NO_CMAKE_PATH PATHS "\$\{CMAKE_PREFIX_PATH\}")
endif()

# Generate the package config file, shared in both build tree and installation usage
write_package_config_file(
  sup-gui-core
  OUTPUT ${PACKAGE_CONFIG_FILE}
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
  DEPENDENCIES Qt${QT_VERSION_MAJOR} sup-dto sup-mvvm sup-gui-extra
  Qt${QT_VERSION_MAJOR}_FIND_OPTIONS ${QT_FIND_OPTIONS} COMPONENTS ${QT_FIND_COMPONENTS})

install(FILES ${PACKAGE_CONFIG_FILE} DESTINATION ${INSTALL_CONFIGDIR})
