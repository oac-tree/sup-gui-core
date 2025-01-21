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
  settings_editor.cpp
  settings_editor.h
  settings_editor_dialog.cpp
  settings_editor_dialog.h
  settings_helper.cpp
  settings_helper.h
  status_bar_helper.cpp
  status_bar_helper.h
)

install(FILES
  main_window_helper.h
  status_bar_helper.h
  settings_helper.h
  settings_editor_dialog.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/mainwindow
)
