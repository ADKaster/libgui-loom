add_library(serenity_resources OBJECT
    SerenitySourceDirResourceImpl.cpp
)

if (TARGET LibDBus)
    add_library(loom_dbus_client OBJECT
        LoomDBusClient.cpp
    )
    target_link_libraries(loom_dbus_client PRIVATE LibDBus)
endif()
