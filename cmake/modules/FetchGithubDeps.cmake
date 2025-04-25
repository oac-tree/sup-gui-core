# Collection of functions used for handling dependencies

if(NOT COA_FETCH_DEPS)
  return()
endif()

message(STATUS "Building dependencies from source")

include(FetchContent)

FetchContent_Declare(
  sup-dto
  GIT_REPOSITORY https://github.com/oac-tree/sup-dto.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  sup-mvvm
  GIT_REPOSITORY https://git.iter.org/scm/coa/sup-mvvm.git
  GIT_TAG 8682a545ab53196bf335ec258cd197746609b3c5
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  sup-gui-extra
  GIT_REPOSITORY https://github.com/oac-tree/sup-gui-extra.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)
