target_sources(${library_name} PRIVATE
  code_editor.cpp
  code_editor.h
)

install(FILES
  code_editor.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/codeeditor
)
