cmake_minimum_required(VERSION 4.0)

project(libgui_loom
    VERSION 0.1.0
    DESCRIPTION "Port of LibGUI to non-Serenity desktops"
    LANGUAGES C CXX
)

if (NOT DEFINED ENV{SERENITY_SOURCE_DIR} OR "$ENV{SERENITY_SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "SERENITY_SOURCE_DIR environment variable is not set.")
endif()

set(SERENITY_SOURCE_DIR $ENV{SERENITY_SOURCE_DIR})
set(SerenityOS_SOURCE_DIR "${SERENITY_SOURCE_DIR}" CACHE STRING "")

list(APPEND CMAKE_MODULE_PATH "${SERENITY_SOURCE_DIR}/Meta/CMake")

if(NOT COMMAND serenity_option)
    macro(serenity_option)
        set(${ARGV})
    endmacro()
endif()


include(check_for_dependencies)
include(use_linker)
include(lagom_options NO_POLICY_SCOPE)
include(setup_ccache)
include(flac_spec_tests)
include(ca_certificates_data)
include(lagom_compile_options)
#include(jakt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(JSClangPlugin INTERFACE)
add_library(GenericClangPlugin INTERFACE)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang$")
    add_compile_options(-Wno-overloaded-virtual)
    add_compile_options(-Wno-unused-private-field)
endif()

include_directories(${SERENITY_SOURCE_DIR})
include_directories(${SERENITY_SOURCE_DIR}/Userland/)
include_directories(${SERENITY_SOURCE_DIR}/Userland/Libraries/)
include_directories(${SERENITY_SOURCE_DIR}/Userland/Services)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Userland/Libraries)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Userland/Services)

option(BUILD_EVERYTHING "Build all optional components" ON)

if (NOT TARGET all_generated)
    # Meta target to run all code-gen steps in the build.
    add_custom_target(all_generated)
endif()

function(lagom_lib target_name fs_name)
    cmake_parse_arguments(LAGOM_LIBRARY "" "LIBRARY_TYPE" "SOURCES;LIBS" ${ARGN})
    string(REPLACE "Lib" "" library ${target_name})
    if (NOT LAGOM_LIBRARY_LIBRARY_TYPE)
        set(LAGOM_LIBRARY_LIBRARY_TYPE "")
    endif()
    add_library(${target_name} ${LAGOM_LIBRARY_LIBRARY_TYPE} ${LAGOM_LIBRARY_SOURCES})
    set_target_properties(
        ${target_name} PROPERTIES
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
        EXPORT_NAME ${library}
        OUTPUT_NAME lagom-${fs_name}
    )
    target_link_libraries(${target_name} PRIVATE ${LAGOM_LIBRARY_LIBS})

    if (NOT "${target_name}" STREQUAL "AK")
        target_link_libraries(${target_name} PRIVATE AK)
    endif()

    serenity_generated_sources(${target_name})
endfunction()

function(lagom_tool tool)
    cmake_parse_arguments(LAGOM_TOOL "" "" "SOURCES;LIBS" ${ARGN})
    add_executable(${tool} ${SOURCES} ${LAGOM_TOOL_SOURCES})
    # alias for parity with exports
    add_executable(Lagom::${tool} ALIAS ${tool})
    target_link_libraries(${tool} AK LibCoreMinimal LibFileSystem GenericClangPlugin ${LAGOM_TOOL_LIBS})
endfunction()

function(serenity_lib name fs_name)
    cmake_parse_arguments(PARSE_ARGV 2 SERENITY_LIB "" "TYPE" "")
    lagom_lib(${name} ${fs_name} LIBRARY_TYPE ${SERENITY_LIB_TYPE} SOURCES ${SOURCES} ${GENERATED_SOURCES})
endfunction()

function(serenity_install_headers dir)
endfunction()

function(serenity_install_sources dir)
endfunction()

macro(add_serenity_subdirectory path)
    add_subdirectory("${SERENITY_SOURCE_DIR}/${path}" "${CMAKE_CURRENT_BINARY_DIR}/${path}")
endmacro()

set(BUILD_LAGOM ON)

add_library(LibC INTERFACE)
add_library(LibCrypt INTERFACE)
add_library(LibSystem INTERFACE)
add_library(NoCoverage INTERFACE)

if (NOT APPLE AND NOT EMSCRIPTEN AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD" AND NOT HAIKU)
    target_link_libraries(LibCrypt INTERFACE crypt) # LibCore::Account uses crypt() but it's not in libcrypt on macOS
endif()

configure_file(${SERENITY_SOURCE_DIR}/AK/Debug.h.in AK/Debug.h @ONLY)
add_serenity_subdirectory(AK)
lagom_lib(AK ak SOURCES ${AK_SOURCES})
find_package(Backtrace)
if (Backtrace_FOUND AND NOT "${Backtrace_LIBRARIES}" STREQUAL "")
    target_link_libraries(AK PRIVATE ${Backtrace_LIBRARIES})
    target_include_directories(AK PRIVATE ${Backtrace_INCLUDE_DIRS})
endif()

find_package(Threads REQUIRED)
add_serenity_subdirectory(Userland/Libraries/LibURL)

add_serenity_subdirectory(Userland/Libraries/LibCore)
target_link_libraries(LibCore PRIVATE LibURL Threads::Threads)

add_serenity_subdirectory(Userland/Libraries/LibFileSystem)
add_serenity_subdirectory(Userland/Libraries/LibMain)
add_serenity_subdirectory(Userland/Libraries/LibTimeZone)
add_serenity_subdirectory(Userland/Libraries/LibIDL)

set(LIBGUI_GML_SOURCES
    GML/Lexer.cpp
    GML/Parser.cpp
)
list(TRANSFORM LIBGUI_GML_SOURCES PREPEND "${SERENITY_SOURCE_DIR}/Userland/Libraries/LibGUI/")
lagom_lib(LibGUI_GML gui_gml
    SOURCES ${LIBGUI_GML_SOURCES}
)

add_serenity_subdirectory(Meta/Lagom/Tools/CodeGenerators)

set(libs
    Config
    ELF
    Gfx
    IPC
    ImageDecoderClient
    Threading
    Unicode
)

foreach(lib IN LISTS libs)
    add_serenity_subdirectory("Userland/Libraries/Lib${lib}")
endforeach()

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/ClipboardClient.ipc Userland/Services/Clipboard/ClipboardClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/ClipboardServer.ipc Userland/Services/Clipboard/ClipboardServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/ConfigServer/ConfigClient.ipc Userland/Services/ConfigServer/ConfigClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/ConfigServer/ConfigServer.ipc Userland/Services/ConfigServer/ConfigServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/NotificationServer/NotificationClient.ipc Userland/Services/NotificationServer/NotificationClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/NotificationServer/NotificationServer.ipc Userland/Services/NotificationServer/NotificationServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowServer.ipc Userland/Services/WindowServer/WindowServerEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowClient.ipc Userland/Services/WindowServer/WindowClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowManagerServer.ipc Userland/Services/WindowServer/WindowManagerServerEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowManagerClient.ipc Userland/Services/WindowServer/WindowManagerClientEndpoint.h)

# HAXity HAX, this won't work at runtime
# Needed by shared class, WindowServer::ScreenLayout
configure_file(${SERENITY_SOURCE_DIR}/Userland/Libraries/LibC/sys/device.h ${CMAKE_CURRENT_BINARY_DIR}/hax/sys/device.h @ONLY)
configure_file(${SERENITY_SOURCE_DIR}/Userland/Libraries/LibC/sys/ioctl.h ${CMAKE_CURRENT_BINARY_DIR}/hax/sys/ioctl.h @ONLY)
configure_file(${SERENITY_SOURCE_DIR}/Userland/Libraries/LibC/sys/devices/gpu.h ${CMAKE_CURRENT_BINARY_DIR}/hax/sys/devices/gpu.h @ONLY)
configure_file(${SERENITY_SOURCE_DIR}/Userland/Libraries/LibC/sys/sysmacros.h ${CMAKE_CURRENT_BINARY_DIR}/hax/sys/sysmacros.h @ONLY)
configure_file(${SERENITY_SOURCE_DIR}/Kernel/API/Ioctl.h ${CMAKE_CURRENT_BINARY_DIR}/hax/Kernel/API/Ioctl.h @ONLY)

find_program(PATCH_PROGRAM patch)
if (NOT PATCH_PROGRAM)
    message(FATAL_ERROR "Required program 'patch' was not found in PATH.")
endif()

set(IOCTL_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/hax/Kernel/API/Ioctl.h")
set(IOCTL_PATCH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/patches/ioctl.patch")

add_custom_command(
    OUTPUT "${IOCTL_HEADER_PATH}.patched"
    COMMAND ${PATCH_PROGRAM} -N "${IOCTL_HEADER_PATH}" -i "${IOCTL_PATCH_PATH}"
    COMMAND ${CMAKE_COMMAND} -E touch "${IOCTL_HEADER_PATH}.patched"
    DEPENDS "${IOCTL_HEADER_PATH}" "${IOCTL_PATCH_PATH}"
    COMMENT "Patching ${IOCTL_HEADER_PATH} with ${IOCTL_PATCH_PATH}"
    VERBATIM
)
add_custom_target(patch_ioctl_header DEPENDS "${IOCTL_HEADER_PATH}.patched")
add_dependencies(all_generated patch_ioctl_header)

## end horrible hax

add_serenity_subdirectory(Userland/Libraries/LibGUI)

add_dependencies(LibGUI patch_ioctl_header)
target_include_directories(LibGUI BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/hax)
