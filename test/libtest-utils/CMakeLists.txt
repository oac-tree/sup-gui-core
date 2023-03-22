set(library_name test-utils)

add_library(${library_name} STATIC "")

add_subdirectory(testutils)

target_link_libraries(${library_name} PUBLIC pthread GTest::gtest GTest::gmock Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test sup-mvvm::view)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SUP_GUI_CORE_AUTOGEN_DIR}>)
