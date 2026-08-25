if (APPLE)
    set(LOOM_BACKEND_DEFAULT AppKit)
else()
    set(LOOM_BACKEND_DEFAULT Wayland)
endif()

set(LOOM_BACKEND ${LOOM_BACKEND_DEFAULT} CACHE STRING "Backend to use")

if (NOT LOOM_BACKEND MATCHES "AppKit|Wayland")
    message(FATAL_ERROR "Invalid LOOM_BACKEND cache value, must be AppKit or Wayland")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (LOOM_BACKEND STREQUAL "Wayland")
    add_subdirectory(LibDBus)
endif()

add_subdirectory(resources)

add_subdirectory(examples)
add_subdirectory(Loom)
