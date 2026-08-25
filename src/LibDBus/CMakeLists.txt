
find_package(DBus1 REQUIRED CONFIG)

add_library(LibDBus
    Bus.cpp
    Connection.cpp
    Error.cpp
    ObjectRegistration.cpp
)

target_link_libraries(LibDBus PRIVATE LibCore)
target_link_libraries(LibDBus PUBLIC ${DBus1_LIBRARIES})
