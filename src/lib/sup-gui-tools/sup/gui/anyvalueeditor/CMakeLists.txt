target_sources(${library_name} PRIVATE
  anyvalue_editor.cpp
  anyvalue_editor.h
  anyvalue_editor_actions.cpp
  anyvalue_editor_actions.h
  anyvalue_editor_context.h
  anyvalue_editor_helper.cpp
  anyvalue_editor_helper.h
  anyvalue_editor_main_window_actions.cpp
  anyvalue_editor_main_window_actions.h
  anyvalue_editor_textpanel.cpp
  anyvalue_editor_textpanel.h
  anyvalue_editor_toolbar.cpp
  anyvalue_editor_toolbar.h
  anyvalue_editor_treepanel.cpp
  anyvalue_editor_treepanel.h
)

add_subdirectory(highlighter)

install(FILES
  anyvalue_editor.h
  anyvalue_editor_context.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/anyvalueeditor
)
