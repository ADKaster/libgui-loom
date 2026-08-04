
if (APPLE)
    set(LOOM_BACKEND_DEFAULT AppKit)
else()
    set(LOOM_BACKEND_DEFAULT Wayland)
endif()

set(LOOM_BACKEND ${LOOM_BACKEND_DEFAULT} CACHE STRING "Backend to use")

if (NOT LOOM_BACKEND MATCHES "AppKit|Wayland")
    message(FATAL_ERROR "Invalid LOOM_BACKEND cache value, must be AppKit or Wayland")
endif()

if (LOOM_BACKEND STREQUAL "AppKit")

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
        AppKit/WindowServerConnectionProxy.mm
        AppKit/ContentView.mm
        AppKit/WindowView.mm
        AppKit/WindowViewController.mm
    )

    target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
    target_compile_options(Loom PRIVATE -fobjc-arc)
    target_compile_definitions(Loom PRIVATE LOOM_BACKEND_APPKIT)
elseif (LOOM_BACKEND STREQUAL "Wayland")

    find_package(Wayland REQUIRED COMPONENTS Client Scanner)

    add_executable(Loom
        Wayland/main.cpp
        Wayland/ScreenLayout.cpp
        Wayland/WindowServerConnectionProxy.cpp
    )
    target_compile_definitions(Loom PRIVATE LOOM_BACKEND_WAYLAND)

    target_link_libraries(Loom PRIVATE Wayland::Client)
endif()

target_sources(Loom PRIVATE
    IPCBridge.cpp
    ClipboardConnectionProxy.cpp
    ConfigServerConnectionProxy.cpp
    LaunchServerConnectionProxy.cpp
)
target_include_directories(Loom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Loom PRIVATE AK LibCore LibIPC LibGfx serenity_resources)

target_sources(Loom PRIVATE
    "${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/Storage.cpp"
)
