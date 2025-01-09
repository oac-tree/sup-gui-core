set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
if (COA_COVERAGE)
  message(STATUS "Enabling test coverage information")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-inline --coverage")
endif()

if(CODAC_FOUND)
  add_definitions(-DCODAC_FOUND)
endif()

add_subdirectory(app)
add_subdirectory(lib)
