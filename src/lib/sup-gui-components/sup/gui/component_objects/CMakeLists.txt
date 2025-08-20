# Collecting here source files that depends on QObject to exclude from Sonar analysis

target_sources(${library_name} PRIVATE
  dto_composer_action_handler.cpp
  dto_composer_action_handler.h
  dto_composer_tab_controller.cpp
  dto_composer_tab_controller.h
  dto_waveform_action_handler.cpp
  dto_waveform_action_handler.h
  proxy_action.cpp
  proxy_action.h
  waveform_editor_action_handler.cpp
  waveform_editor_action_handler.h
  waveform_twocolumn_viewmodel.cpp
  waveform_twocolumn_viewmodel.h
)

