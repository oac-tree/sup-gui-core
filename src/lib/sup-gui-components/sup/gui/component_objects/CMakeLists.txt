# Collecting here source files that depends on QObject to exclude from Sonar analysis

target_sources(${library_name} PRIVATE
  dto_composer_action_handler.cpp
  dto_composer_action_handler.h
  dto_composer_tab_controller.cpp
  dto_composer_tab_controller.h
  diff_highlight_controller.cpp
  diff_highlight_controller.h
  dto_waveform_action_handler.cpp
  dto_waveform_action_handler.h
  proxy_action.cpp
  proxy_action.h
  scroll_area_mirror_controller.cpp
  scroll_area_mirror_controller.h
  tab_widget_controller.cpp
  tab_widget_controller.h
  tree_expand_controller.cpp
  tree_expand_controller.h
  waveform_editor_action_handler.cpp
  waveform_editor_action_handler.h
  waveform_twocolumn_viewmodel.cpp
  waveform_twocolumn_viewmodel.h
)

