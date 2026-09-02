
find_package(DBus1 REQUIRED CONFIG)

lagom_lib(LibDBus dbus
    SOURCES
        Bus.cpp
        Connection.cpp
        Error.cpp
        ObjectRegistration.cpp
)

target_link_libraries(LibDBus PRIVATE LibCore)
target_link_libraries(LibDBus PUBLIC ${DBus1_LIBRARIES})
