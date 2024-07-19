target_sources(${library_name} PRIVATE
  code_editor.cpp
  code_editor.h
  code_editor_sidebar.cpp
  code_editor_sidebar.h
  code_view.cpp
  code_view.h
)

install(FILES
  code_editor.h
  code_view.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/codeeditor
)
