
include(ExternalProject)
include(FindPackageHandleStandardArgs)
find_package(PkgConfig REQUIRED)

set(WAYLAND_STAGE_DIR ${CMAKE_BINARY_DIR}/wayland_staging)
set(WAYLAND_FALLBACK_VERSION 1.26.0)

set(_wayland_known_components Client Server Cursor Egl Scanner)
if (Wayland_FIND_COMPONENTS)
    set(_wayland_components ${Wayland_FIND_COMPONENTS})
else()
    set(_wayland_components ${_wayland_known_components})
endif()

foreach(_wayland_component IN LISTS _wayland_known_components)
    set(Wayland_${_wayland_component}_FOUND FALSE)
endforeach()

function(_wayland_add_library_target component target_name soname)
    if (TARGET ${target_name})
        return()
    endif()

    if (_wayland_use_external)
        add_library(${target_name} SHARED IMPORTED GLOBAL)
        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${WAYLAND_STAGE_DIR}/lib/lib${soname}${CMAKE_SHARED_LIBRARY_SUFFIX}"
            INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_STAGE_DIR}/include"
        )
        if (TARGET Wayland_external-install)
            add_dependencies(${target_name} Wayland_external-install)
        elseif (TARGET Wayland_external)
            add_dependencies(${target_name} Wayland_external)
        endif()
        return()
    endif()

    if (NOT TARGET PkgConfig::PC_Wayland_${component})
        return()
    endif()

    add_library(${target_name} INTERFACE IMPORTED GLOBAL)
    target_link_libraries(${target_name} INTERFACE PkgConfig::PC_Wayland_${component})
endfunction()

function(_wayland_add_scanner_target target_name scanner_executable)
    if (TARGET ${target_name})
        return()
    endif()
    add_executable(${target_name} IMPORTED GLOBAL)
    set_target_properties(${target_name} PROPERTIES
        IMPORTED_LOCATION "${scanner_executable}"
    )
    if (TARGET Wayland_external-install)
        add_dependencies(${target_name} Wayland_external-install)
    elseif (TARGET Wayland_external)
        add_dependencies(${target_name} Wayland_external)
    endif()
endfunction()

set(_wayland_can_use_pkgconfig TRUE)
pkg_check_modules(PC_Wayland_Client QUIET IMPORTED_TARGET wayland-client)
pkg_check_modules(PC_Wayland_Server QUIET IMPORTED_TARGET wayland-server)
pkg_check_modules(PC_Wayland_Cursor QUIET IMPORTED_TARGET wayland-cursor)
pkg_check_modules(PC_Wayland_Egl QUIET IMPORTED_TARGET wayland-egl)
pkg_check_modules(PC_Wayland_Scanner QUIET wayland-scanner)

if (PC_Wayland_Client_FOUND)
    set(Wayland_Client_FOUND TRUE)
endif()
if (PC_Wayland_Server_FOUND)
    set(Wayland_Server_FOUND TRUE)
endif()
if (PC_Wayland_Cursor_FOUND)
    set(Wayland_Cursor_FOUND TRUE)
endif()
if (PC_Wayland_Egl_FOUND)
    set(Wayland_Egl_FOUND TRUE)
endif()
if (PC_Wayland_Scanner_FOUND)
    pkg_get_variable(_Wayland_SCANNER_FROM_PC wayland-scanner wayland_scanner)
    if (_Wayland_SCANNER_FROM_PC)
        set(Wayland_SCANNER_EXECUTABLE "${_Wayland_SCANNER_FROM_PC}")
    else()
        pkg_get_variable(_Wayland_SCANNER_BINDIR wayland-scanner bindir)
        find_program(Wayland_SCANNER_EXECUTABLE NAMES wayland-scanner HINTS "${_Wayland_SCANNER_BINDIR}")
    endif()
else()
    find_program(Wayland_SCANNER_EXECUTABLE NAMES wayland-scanner)
endif()
if (Wayland_SCANNER_EXECUTABLE)
    set(Wayland_Scanner_FOUND TRUE)
endif()

foreach(_wayland_component IN LISTS _wayland_components)
    if (NOT Wayland_${_wayland_component}_FOUND)
        set(_wayland_can_use_pkgconfig FALSE)
    endif()
endforeach()

if (_wayland_can_use_pkgconfig)
    set(_wayland_use_external FALSE)

    set(Wayland_VERSION ${PC_Wayland_Client_VERSION})
    set(Wayland_INCLUDE_DIRS)
    foreach(_wayland_library_component IN ITEMS Client Server Cursor Egl)
        if (PC_Wayland_${_wayland_library_component}_FOUND)
            list(APPEND Wayland_INCLUDE_DIRS ${PC_Wayland_${_wayland_library_component}_INCLUDE_DIRS})
        endif()
    endforeach()
    list(REMOVE_DUPLICATES Wayland_INCLUDE_DIRS)
    if (Wayland_INCLUDE_DIRS)
        list(GET Wayland_INCLUDE_DIRS 0 Wayland_INCLUDE_DIR)
    endif()
    set(Wayland_DEFINITIONS ${PC_Wayland_Client_CFLAGS_OTHER})

    set(Wayland_CLIENT_LIBRARY "${PC_Wayland_Client_LINK_LIBRARIES}")
    set(Wayland_SERVER_LIBRARY "${PC_Wayland_Server_LINK_LIBRARIES}")
    set(Wayland_CURSOR_LIBRARY "${PC_Wayland_Cursor_LINK_LIBRARIES}")
    set(Wayland_EGL_LIBRARY "${PC_Wayland_Egl_LINK_LIBRARIES}")

    if (PC_Wayland_Scanner_FOUND)
        pkg_get_variable(Wayland_DATADIR wayland-scanner pkgdatadir)
    endif()
else()
    set(Wayland_VERSION ${WAYLAND_FALLBACK_VERSION})
    set(_wayland_use_external TRUE)
    file(MAKE_DIRECTORY "${WAYLAND_STAGE_DIR}/include" "${WAYLAND_STAGE_DIR}/lib" "${WAYLAND_STAGE_DIR}/bin" "${WAYLAND_STAGE_DIR}/share/wayland")

    find_program(MESON meson)
    if (NOT MESON)
        message(FATAL_ERROR "Failed to find meson, which is required for fallback wayland build")
    endif()
    find_program(NINJA ninja)
    if (NOT NINJA)
        message(FATAL_ERROR "Failed to find ninja, which is required for fallback wayland build")
    endif()

    ExternalProject_Add(Wayland_external
        GIT_REPOSITORY https://gitlab.freedesktop.org/wayland/wayland.git
        GIT_TAG ${WAYLAND_FALLBACK_VERSION}

        CONFIGURE_COMMAND ${MESON} setup <BINARY_DIR> <SOURCE_DIR>
            --prefix=${WAYLAND_STAGE_DIR}
            --buildtype=release
            -Ddocumentation=false
            -Dtests=false
        BUILD_COMMAND ${NINJA} -C <BINARY_DIR>
        INSTALL_COMMAND ${NINJA} -C <BINARY_DIR> install
        INSTALL_BYPRODUCTS
            "${WAYLAND_STAGE_DIR}/lib/libwayland-client${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "${WAYLAND_STAGE_DIR}/lib/libwayland-server${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "${WAYLAND_STAGE_DIR}/lib/libwayland-cursor${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "${WAYLAND_STAGE_DIR}/lib/libwayland-egl${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "${WAYLAND_STAGE_DIR}/include/wayland-client-core.h"
            "${WAYLAND_STAGE_DIR}/bin/wayland-scanner"
        STEP_TARGETS configure build install
    )

    set(Wayland_Client_FOUND TRUE)
    set(Wayland_Server_FOUND TRUE)
    set(Wayland_Cursor_FOUND TRUE)
    set(Wayland_Egl_FOUND TRUE)
    set(Wayland_Scanner_FOUND TRUE)

    set(Wayland_INCLUDE_DIR "${WAYLAND_STAGE_DIR}/include")
    set(Wayland_INCLUDE_DIRS "${Wayland_INCLUDE_DIR}")
    set(Wayland_DEFINITIONS "")
    set(Wayland_CLIENT_LIBRARY "${WAYLAND_STAGE_DIR}/lib/libwayland-client${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(Wayland_SERVER_LIBRARY "${WAYLAND_STAGE_DIR}/lib/libwayland-server${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(Wayland_CURSOR_LIBRARY "${WAYLAND_STAGE_DIR}/lib/libwayland-cursor${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(Wayland_EGL_LIBRARY "${WAYLAND_STAGE_DIR}/lib/libwayland-egl${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(Wayland_DATADIR "${WAYLAND_STAGE_DIR}/share/wayland")
    set(Wayland_SCANNER_EXECUTABLE "${WAYLAND_STAGE_DIR}/bin/wayland-scanner")
endif()

_wayland_add_library_target(Client wayland::wayland-client wayland-client)
_wayland_add_library_target(Server wayland::wayland-server wayland-server)
_wayland_add_library_target(Cursor wayland::wayland-cursor wayland-cursor)
_wayland_add_library_target(Egl wayland::wayland-egl wayland-egl)

if (NOT TARGET Wayland::Client)
    add_library(Wayland::Client INTERFACE IMPORTED GLOBAL)
    target_link_libraries(Wayland::Client INTERFACE wayland::wayland-client)
endif()
if (NOT TARGET Wayland::Server)
    add_library(Wayland::Server INTERFACE IMPORTED GLOBAL)
    target_link_libraries(Wayland::Server INTERFACE wayland::wayland-server)
endif()
if (NOT TARGET Wayland::Cursor)
    add_library(Wayland::Cursor INTERFACE IMPORTED GLOBAL)
    target_link_libraries(Wayland::Cursor INTERFACE wayland::wayland-cursor)
endif()
if (NOT TARGET Wayland::Egl)
    add_library(Wayland::Egl INTERFACE IMPORTED GLOBAL)
    target_link_libraries(Wayland::Egl INTERFACE wayland::wayland-egl)
endif()
_wayland_add_scanner_target(wayland::wayland-scanner "${Wayland_SCANNER_EXECUTABLE}")
if (NOT TARGET Wayland::Scanner)
    _wayland_add_scanner_target(Wayland::Scanner "${Wayland_SCANNER_EXECUTABLE}")
endif()

set(Wayland_LIBRARIES)
set(Wayland_TARGETS)
foreach(_wayland_component IN LISTS _wayland_components)
    if (_wayland_component STREQUAL "Client")
        list(APPEND Wayland_LIBRARIES Wayland::Client)
        list(APPEND Wayland_TARGETS Wayland::Client)
    elseif (_wayland_component STREQUAL "Server")
        list(APPEND Wayland_LIBRARIES Wayland::Server)
        list(APPEND Wayland_TARGETS Wayland::Server)
    elseif (_wayland_component STREQUAL "Cursor")
        list(APPEND Wayland_LIBRARIES Wayland::Cursor)
        list(APPEND Wayland_TARGETS Wayland::Cursor)
    elseif (_wayland_component STREQUAL "Egl")
        list(APPEND Wayland_LIBRARIES Wayland::Egl)
        list(APPEND Wayland_TARGETS Wayland::Egl)
    elseif (_wayland_component STREQUAL "Scanner")
        list(APPEND Wayland_TARGETS Wayland::Scanner)
    endif()
endforeach()

find_package_handle_standard_args(Wayland
    FOUND_VAR Wayland_FOUND
    REQUIRED_VARS Wayland_TARGETS
    VERSION_VAR Wayland_VERSION
    HANDLE_COMPONENTS
)

mark_as_advanced(
    Wayland_INCLUDE_DIR
    Wayland_CLIENT_LIBRARY
    Wayland_SERVER_LIBRARY
    Wayland_CURSOR_LIBRARY
    Wayland_EGL_LIBRARY
    Wayland_SCANNER_EXECUTABLE
)
