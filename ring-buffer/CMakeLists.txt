add_executable(ring_buffer_app
    main.cpp
    ring_buffer.cpp
    interrupts.cpp
)

target_include_directories(ring_buffer_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})