
if (APPLE)

    find_library(APPKIT_FRAMEWORK AppKit REQUIRED)

    add_executable(Loom
        MACOSX_BUNDLE
        AppKit/main.mm
        AppKit/Application.mm
        AppKit/ApplicationDelegate.mm
        AppKit/Conversions.mm
        AppKit/ScreenLayout.mm
        AppKit/EventLoopImplementation.mm
        AppKit/WindowController.mm
        AppKit/Window.mm
        AppKit/IconBag.mm
        AppKit/WindowServerCallbacksMacOS.mm
        AppKit/ContentView.mm
        AppKit/WindowView.mm
        AppKit/WindowViewController.mm
    )

    target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
    target_compile_options(Loom PRIVATE -fobjc-arc)
endif()

target_sources(Loom PRIVATE
    IPCBridge.cpp
    ClipboardConnectionProxy.cpp
    ConfigServerConnectionProxy.cpp
    LaunchServerConnectionProxy.cpp
    WindowServerConnectionProxy.cpp
)
target_include_directories(Loom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Loom PRIVATE AK LibCore LibIPC LibGfx serenity_resources)

target_sources(Loom PRIVATE
    "${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/Storage.cpp"
)
