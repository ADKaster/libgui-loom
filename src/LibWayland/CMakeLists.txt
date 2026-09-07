find_package(PkgConfig REQUIRED QUIET)
find_package(Wayland REQUIRED COMPONENTS Client Protocols Scanner)
pkg_check_modules(XKBCommon REQUIRED IMPORTED_TARGET xkbcommon)

function(generate_wayland_protocol protocol_name sources_var)
    cmake_parse_arguments(PARSE_ARGV 2 PROTOCOL "" "FILE_PREFIX;STABILITY;BASENAME" "")

    if (NOT PROTOCOL_FILE_PREFIX)
        set(PROTOCOL_FILE_PREFIX ${protocol_name})
    endif()

    if (NOT PROTOCOL_STABILITY)
        set(PROTOCOL_STABILITY "stable")
    endif()

    if (NOT PROTOCOL_BASENAME)
        set(PROTOCOL_BASENAME ${protocol_name})
    endif()

    set(PROTOCOL_XML "${Wayland_Protocols_DATADIR}/${PROTOCOL_STABILITY}/${PROTOCOL_BASENAME}/${protocol_name}.xml")
    add_custom_command(
        OUTPUT wayland-protocols/${PROTOCOL_FILE_PREFIX}-client.h
        COMMAND ${Wayland_SCANNER_EXECUTABLE} client-header
            ${PROTOCOL_XML}
            ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/${PROTOCOL_FILE_PREFIX}-client.h
        DEPENDS Wayland::Protocols Wayland::Scanner
        COMMENT "Generating ${PROTOCOL_FILE_PREFIX} client header"
    )
    add_custom_command(
        OUTPUT wayland-protocols/${PROTOCOL_FILE_PREFIX}.c
        COMMAND ${Wayland_SCANNER_EXECUTABLE} private-code
            ${PROTOCOL_XML}
            ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/${PROTOCOL_FILE_PREFIX}.c
        DEPENDS Wayland::Protocols Wayland::Scanner
        COMMENT "Generating ${PROTOCOL_FILE_PREFIX} protocol source"
    )

    set(${sources_var}
        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/${PROTOCOL_FILE_PREFIX}-client.h
        ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols/${PROTOCOL_FILE_PREFIX}.c
        PARENT_SCOPE
    )

    string(REPLACE "-" "_" protocol_target_name ${PROTOCOL_BASENAME})
    add_custom_target(generate_${protocol_target_name}
        DEPENDS wayland-protocols/${PROTOCOL_FILE_PREFIX}-client.h
                wayland-protocols/${PROTOCOL_FILE_PREFIX}.c
    )
endfunction()

lagom_lib(LibWayland wayland
    SOURCES
        Buffer.cpp
        Callback.cpp
        Compositor.cpp
        Display.cpp
        Fixes.cpp
        Keyboard.cpp
        Output.cpp
        Pointer.cpp
        Registry.cpp
        Region.cpp
        Seat.cpp
        Shm.cpp
        ShmPool.cpp
        Surface.cpp
        Viewporter.cpp
        Viewport.cpp
        XdgPositioner.cpp
        XdgPopup.cpp
        XdgSurface.cpp
        XdgToplevel.cpp
        XdgWmBase.cpp
)

generate_wayland_protocol(xdg-shell XDG_SHELL_SOURCES)
generate_wayland_protocol(viewporter VIEWPORTER_SOURCES FILE_PREFIX wp-viewporter)

target_sources(LibWayland PRIVATE
    ${XDG_SHELL_SOURCES}
    ${VIEWPORTER_SOURCES}
)
add_dependencies(LibWayland
    generate_xdg_shell
    generate_viewporter
)

target_include_directories(LibWayland SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols)
target_link_libraries(LibWayland PUBLIC Wayland::Client PkgConfig::XKBCommon)
