find_package(PkgConfig REQUIRED QUIET)
find_package(Wayland REQUIRED COMPONENTS Client Protocols Scanner)
pkg_check_modules(XKBCommon REQUIRED IMPORTED_TARGET xkbcommon)

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

lagom_lib(LibWayland wayland
    SOURCES
        Display.cpp
        Callback.cpp
        Registry.cpp
        Buffer.cpp
        Compositor.cpp
        Fixes.cpp
        Keyboard.cpp
        Output.cpp
        Pointer.cpp
        Seat.cpp
        Shm.cpp
        ShmPool.cpp
        Surface.cpp
        XdgPositioner.cpp
        XdgPopup.cpp
        XdgSurface.cpp
        XdgToplevel.cpp
        XdgWmBase.cpp

        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell.c
        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell-client.h
)

target_include_directories(LibWayland SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols)
target_link_libraries(LibWayland PUBLIC Wayland::Client)
target_link_libraries(LibWayland PRIVATE PkgConfig::XKBCommon)
