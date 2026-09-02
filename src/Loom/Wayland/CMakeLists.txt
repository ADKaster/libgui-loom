
add_executable(Loom
    main.cpp
    Application.cpp
    Button.cpp
    ScreenLayout.cpp
    Window.cpp
    WindowFrame.cpp
    WindowServerConnectionProxy.cpp
)
target_compile_definitions(Loom PRIVATE LOOM_BACKEND_WAYLAND)

target_link_libraries(Loom PRIVATE LibWayland LibDBus LibMain)

set(LOOM_EXE_LOCATION $<TARGET_FILE_DIR:Loom>)
set(LOOM_SERVICE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../meta/org.serenityos.Loom.service.in)
configure_file(${LOOM_SERVICE_TEMPLATE}  ${CMAKE_CURRENT_BINARY_DIR}/org.serenityos.Loom.service.cfg @ONLY)
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbus-1/services/org.serenityos.Loom.service"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/org.serenityos.Loom.service.cfg"
)
