
add_executable(BasicApp
    main.cpp
)
target_link_libraries(BasicApp PRIVATE LibMain LibGUI LibGfx serenity_resources)

if (TARGET loom_dbus_client)
    target_link_libraries(BasicApp PRIVATE loom_dbus_client)
endif()
