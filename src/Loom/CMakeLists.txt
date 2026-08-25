
add_subdirectory(${LOOM_BACKEND})

target_sources(Loom PRIVATE
    IPCBridge.cpp
    ClipboardConnectionProxy.cpp
    ConfigServerConnectionProxy.cpp
    LaunchServerConnectionProxy.cpp
)
target_link_libraries(Loom PRIVATE AK LibCore LibIPC LibGfx serenity_resources)

target_sources(Loom PRIVATE
    "${SERENITY_SOURCE_DIR}/Userland/Services/Clipboard/Storage.cpp"
)
