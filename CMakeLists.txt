cmake_minimum_required(VERSION 4.0)

project(libgui_loom
    VERSION 0.1.0
    DESCRIPTION "Port of LibGUI to non-Serenity desktops"
    LANGUAGES C CXX
)

if (APPLE)
    enable_language(OBJCXX)
endif()

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
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Userland/)
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

function(serenity_app target_name)
    cmake_parse_arguments(PARSE_ARGV 1 SERENITY_APP "" "ICON" "")

    set(small_icon "${SerenityOS_SOURCE_DIR}/Base/res/icons/16x16/${SERENITY_APP_ICON}.png")
    set(medium_icon "${SerenityOS_SOURCE_DIR}/Base/res/icons/32x32/${SERENITY_APP_ICON}.png")

    add_executable(${target_name} ${SOURCES})
    serenity_generated_sources(${target_name})

    if (NOT EXISTS "${small_icon}")
        message(FATAL_ERROR "Missing small app icon: ${small_icon}")
    endif()

    if (NOT EXISTS "${medium_icon}")
        # These icons are designed small only for use in applets, and thus are exempt.
        list(APPEND allowed_missing_medium_icons "audio-volume-high")
        list(APPEND allowed_missing_medium_icons "edit-copy")

        if (NOT ${SERENITY_APP_ICON} IN_LIST allowed_missing_medium_icons)
            message(FATAL_ERROR "Missing medium app icon: ${medium_icon}")
        endif()
    endif()

    # FIXME: Create lists of 'required resources' per app and copy them into the app bundle
    target_link_libraries(${target_name} PRIVATE serenity_resources)

    set_target_properties(${target_name} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.SerenityOS.${target_name}"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.SerenityOS.${target_name}"
    )

    if (APPLE)
        find_program(ICONUTIL iconutil)
        find_program(MAGICK magick)
        if (ICONUTIL AND MAGICK)
            # FIXME: We could probably just write a python script to do all of this
            add_custom_command(OUTPUT "app_icon.icns"
                COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${small_icon}" "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_16x16.png"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${medium_icon}" "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_16x16@2x.png"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${medium_icon}" "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_32x32.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 64x64\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_32x32@2x.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 128x128\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_128x128.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 256x256\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_128x128@2x.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 256x256\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_256x256.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 512x512\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_256x256@2x.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 512x512\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_512x512.png"
                COMMAND "${MAGICK}" "${medium_icon}" -resize 1024x1024\! -filter point "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset/icon_512x512@2x.png"
                COMMAND "${ICONUTIL}" --convert icns "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.iconset" --output "${CMAKE_CURRENT_BINARY_DIR}/app_icon.tmp.icns"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/app_icon.tmp.icns" "${CMAKE_CURRENT_BINARY_DIR}/app_icon.icns"
                VERBATIM
                DEPENDS "${small_icon}" "${medium_icon}"
            )
            add_custom_target("${target_name}_icon" DEPENDS "app_icon.icns")
            add_dependencies(${target_name} "${target_name}_icon")

            target_sources("${target_name}" PRIVATE "app_icon.icns")
            set_source_files_properties("app_icon.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endif()

        configure_file("${libgui_loom_SOURCE_DIR}/meta/Info.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" @ONLY)
        set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")
    endif()
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

macro(add_dummy_component name)
    if (NOT TARGET ${name})
        add_library(${name} INTERFACE)
        serenity_component(${name} RECOMMENDED TARGETS ${name})
    endif()
endmacro()

add_dummy_component(ImageDecoder)
add_dummy_component(FileSystemAccessServer)
add_dummy_component(WindowServer)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/ClipboardClient.ipc Userland/Services/Clipboard/ClipboardClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/ClipboardServer.ipc Userland/Services/Clipboard/ClipboardServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/ConfigServer/ConfigClient.ipc Userland/Services/ConfigServer/ConfigClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/ConfigServer/ConfigServer.ipc Userland/Services/ConfigServer/ConfigServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/FileSystemAccessServer/FileSystemAccessClient.ipc Userland/Services/FileSystemAccessServer/FileSystemAccessClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/FileSystemAccessServer/FileSystemAccessServer.ipc Userland/Services/FileSystemAccessServer/FileSystemAccessServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/LaunchServer/LaunchClient.ipc Userland/Services/LaunchServer/LaunchClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/LaunchServer/LaunchServer.ipc Userland/Services/LaunchServer/LaunchServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/NotificationServer/NotificationClient.ipc Userland/Services/NotificationServer/NotificationClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/NotificationServer/NotificationServer.ipc Userland/Services/NotificationServer/NotificationServerEndpoint.h)

compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowServer.ipc Userland/Services/WindowServer/WindowServerEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowClient.ipc Userland/Services/WindowServer/WindowClientEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowManagerServer.ipc Userland/Services/WindowServer/WindowManagerServerEndpoint.h)
compile_ipc(${SERENITY_SOURCE_DIR}/Userland/Services/WindowServer/WindowManagerClient.ipc Userland/Services/WindowServer/WindowManagerClientEndpoint.h)

set(libs
    Config
    Compress
    Crypto
    Desktop
    ELF
    FileSystemAccessClient
    Gfx
    GUI
    IPC
    ImageDecoderClient
    Regex
    RIFF
    Syntax
    TextCodec
    Threading
    Unicode
)

foreach(lib IN LISTS libs)
    add_serenity_subdirectory("Userland/Libraries/Lib${lib}")
endforeach()

add_subdirectory(src)

add_serenity_subdirectory(Userland/Applications/Calculator)
add_serenity_subdirectory(Userland/Applications/PixelPaint)
