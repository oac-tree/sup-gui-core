target_sources(${library_name} PRIVATE
    views_metatypes.cpp
    views_metatypes.h
)

add_subdirectory(views)
add_subdirectory(widgets)
add_subdirectory(mainwindow)
