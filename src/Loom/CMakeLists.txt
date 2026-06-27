
if (APPLE)
    add_executable(Loom
        MACOSX_BUNDLE
        AppKit/main.mm
        AppKit/Application.mm
        AppKit/ApplicationDelegate.mm
        AppKit/Conversions.mm
        AppKit/ScreenLayout.mm
        AppKit/EventLoopImplementation.mm
        AppKit/WindowController.mm
        AppKit/WindowServerCallbacksMacOS.mm
    )

    target_include_directories(Loom BEFORE PRIVATE
        ${PROJECT_SOURCE_DIR}/extras
    )

    target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
    target_compile_options(Loom PRIVATE -fobjc-arc)
endif()

target_sources(Loom PRIVATE
    IPCBridge.cpp
    ClipboardConnectionProxy.cpp
    WindowServerConnectionProxy.cpp
)
target_include_directories(Loom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Loom PRIVATE AK LibCore LibIPC LibGfx)
