
target_sources(${library_name} PRIVATE
  anyvalue_waveform_editor.cpp
  anyvalue_waveform_editor.h
  waveform_editor.cpp
  waveform_editor.h
  waveform_editor_actions.cpp
  waveform_editor_actions.h
  waveform_editor_toolbar.cpp
  waveform_editor_toolbar.h
  waveform_editor_widget.cpp
  waveform_editor_widget.h
  waveform_table_widget.cpp
  waveform_table_widget.h
)

install(FILES
  waveform_editor.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/gui/views/waveformeditor
)
