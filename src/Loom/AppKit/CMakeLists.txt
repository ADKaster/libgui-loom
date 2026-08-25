
find_library(APPKIT_FRAMEWORK AppKit REQUIRED)

add_executable(Loom
    MACOSX_BUNDLE
    main.mm
    Application.mm
    ApplicationDelegate.mm
    Conversions.mm
    ScreenLayout.mm
    EventLoopImplementation.mm
    WindowController.mm
    Window.mm
    IconBag.mm
    WindowServerConnectionProxy.mm
    ContentView.mm
    WindowView.mm
    WindowViewController.mm
)

target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
target_compile_options(Loom PRIVATE -fobjc-arc)
target_compile_definitions(Loom PRIVATE LOOM_BACKEND_APPKIT)
