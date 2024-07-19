# Collection of general purpose widgets with the dependency on Qt only.

target_sources(${library_name} PRIVATE
  anyvalue_clipboard_utils.cpp
  anyvalue_clipboard_utils.h
  about_application_dialog.cpp
  about_application_dialog.h
  action_menu.cpp
  action_menu.h
  collapsible_list_view.cpp
  collapsible_list_view.h
  collapsible_toolbar.cpp
  collapsible_toolbar.h
  collapsible_widget.cpp
  collapsible_widget.h
  custom_header_view.cpp
  custom_header_view.h
  message_helper.cpp
  message_helper.h
  item_stack_widget.cpp
  item_stack_widget.h
  panel_toolbar.cpp
  panel_toolbar.h
  style_utils.cpp
  style_utils.h
  tree_helper.cpp
  tree_helper.h
standard_message_handlers.cpp
standard_message_handlers.h

  )
