target_sources(${library_name} PRIVATE
  abstract_anyvalue_editor.h
  anyvalue_editor.cpp
  anyvalue_editor.h
  anyvalue_editor_actions.cpp
  anyvalue_editor_actions.h
  anyvalue_editor_dialog.cpp
  anyvalue_editor_dialog.h
  anyvalue_editor_dialog_factory.cpp
  anyvalue_editor_dialog_factory.h
  anyvalue_editor_textpanel.cpp
  anyvalue_editor_textpanel.h
  anyvalue_editor_treepanel.cpp
  anyvalue_editor_treepanel.h
  anyvalue_editor_widget.cpp
  anyvalue_editor_widget.h
  command_line_options.cpp
  command_line_options.h
)

install(FILES
  anyvalue_editor.h
  abstract_anyvalue_editor.h
  anyvalue_editor_dialog.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/views/anyvalueeditor
)
