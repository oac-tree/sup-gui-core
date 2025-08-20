# Collecting here source files that depends on QObject to exclude from Sonar analysis

target_sources(${library_name} PRIVATE
  dto_composer_action_handler.cpp
  dto_composer_action_handler.h
  dto_waveform_action_handler.cpp
  dto_waveform_action_handler.h
  proxy_action.cpp
  proxy_action.h
)

