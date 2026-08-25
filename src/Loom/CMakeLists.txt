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

    find_package(Wayland REQUIRED COMPONENTS Client Protocols Scanner)

    set(XDG_SHELL_XML "${Wayland_Protocols_DATADIR}/stable/xdg-shell/xdg-shell.xml")
    add_custom_command(
        OUTPUT wayland-protocols/xdg-shell-client.h
        COMMAND ${Wayland_SCANNER_EXECUTABLE} client-header
                ${XDG_SHELL_XML}
                ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell-client.h
        DEPENDS Wayland::Protocols
        COMMENT "Generating xdg-shell client header"
    )
    add_custom_command(
        OUTPUT wayland-protocols/xdg-shell.c
        COMMAND ${Wayland_SCANNER_EXECUTABLE} private-code
                ${XDG_SHELL_XML}
                ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell.c
        DEPENDS Wayland::Protocols
        COMMENT "Generating xdg-shell protocol source"
    )

    add_executable(Loom
        Wayland/main.cpp
        Wayland/Display.cpp
        Wayland/Window.cpp
        Wayland/ScreenLayout.cpp
        Wayland/WindowServerConnectionProxy.cpp
        Wayland/Protocol/Callback.cpp
        Wayland/Protocol/Registry.cpp
        Wayland/Protocol/Buffer.cpp
        Wayland/Protocol/Compositor.cpp
        Wayland/Protocol/Fixes.cpp
        Wayland/Protocol/Output.cpp
        Wayland/Protocol/Shm.cpp
        Wayland/Protocol/ShmPool.cpp
        Wayland/Protocol/Surface.cpp
        Wayland/Protocol/XdgSurface.cpp
        Wayland/Protocol/XdgToplevel.cpp
        Wayland/Protocol/XdgWmBase.cpp

        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell.c
        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell-client.h
    )
    target_compile_definitions(Loom PRIVATE LOOM_BACKEND_WAYLAND)
    target_include_directories(Loom SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols)

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
