set(sources
    src/app.cpp
    src/document_manager.cpp
    src/input_manager.cpp
    src/renderer.cpp
    src/svg_icon.cpp
    src/ui/ui_elements.cpp
    src/ui/ui_manager.cpp
    src/ui/ui_main_menu.cpp
    src/ui/ui_document_tab.cpp
    src/ui/ui_modal_dialogs.cpp
)

set(exe_sources
    ${sources}
    src/main.cpp
)