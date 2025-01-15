set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

if(CODAC_FOUND)
  add_definitions(-DCODAC_FOUND)
endif()

add_subdirectory(app)
add_subdirectory(lib)
