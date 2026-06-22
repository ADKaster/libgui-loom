
if (APPLE)
    add_executable(Loom
        MACOSX_BUNDLE
        AppKit/main.mm
        AppKit/ApplicationDelegate.mm
        AppKit/ScreenLayout.mm
        AppKit/EventLoopImplementation.mm
    )

    target_include_directories(Loom BEFORE PRIVATE
        ${PROJECT_SOURCE_DIR}/extras
    )

    target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
    target_compile_options(Loom PRIVATE -fobjc-arc)
endif()

target_sources(Loom PRIVATE
    WindowServerConnectionProxy.cpp
)
target_link_libraries(Loom PRIVATE LibCore LibIPC LibGfx)
