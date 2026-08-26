
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
    main.cpp
    Application.cpp
    Display.cpp
    Window.cpp
    ScreenLayout.cpp
    WindowServerConnectionProxy.cpp
    Protocol/Callback.cpp
    Protocol/Registry.cpp
    Protocol/Buffer.cpp
    Protocol/Compositor.cpp
    Protocol/Fixes.cpp
    Protocol/Output.cpp
    Protocol/Shm.cpp
    Protocol/ShmPool.cpp
    Protocol/Surface.cpp
    Protocol/XdgSurface.cpp
    Protocol/XdgToplevel.cpp
    Protocol/XdgWmBase.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell.c
    ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/xdg-shell-client.h
)
target_compile_definitions(Loom PRIVATE LOOM_BACKEND_WAYLAND)
target_include_directories(Loom SYSTEM PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols)

target_link_libraries(Loom PRIVATE Wayland::Client LibDBus LibMain)

set(LOOM_EXE_LOCATION $<TARGET_FILE_DIR:Loom>)
set(LOOM_SERVICE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../meta/org.serenityos.Loom.service.in)
configure_file(${LOOM_SERVICE_TEMPLATE}  ${CMAKE_CURRENT_BINARY_DIR}/org.serenityos.Loom.service.cfg @ONLY)
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbus-1/services/org.serenityos.Loom.service"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/org.serenityos.Loom.service.cfg"
)
