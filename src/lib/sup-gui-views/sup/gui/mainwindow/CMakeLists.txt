target_sources(${library_name} PRIVATE
  about_application_dialog.cpp
  about_application_dialog.h
  anyvalue_editor_main_window.cpp
  anyvalue_editor_main_window.h
  anyvalue_editor_main_window_actions.cpp
  anyvalue_editor_main_window_actions.h
  color_palette.cpp
  color_palette.h
  dto_editor_main_window.cpp
  dto_editor_main_window.h
  dto_editor_main_window_actions.cpp
  dto_editor_main_window_actions.h
  main_window_helper.cpp
  main_window_helper.h
  run_application.h
)

install(FILES
  main_window_helper.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/mainwindow
)
