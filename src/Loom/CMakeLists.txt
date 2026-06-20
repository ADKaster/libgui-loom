add_executable(Loom
    MACOSX_BUNDLE
    main.mm
)

target_link_libraries(Loom PRIVATE ${APPKIT_FRAMEWORK})
target_compile_options(Loom PRIVATE -fobjc-arc)
