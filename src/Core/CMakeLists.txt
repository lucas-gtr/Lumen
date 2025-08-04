add_library(Core STATIC
    Framebuffer.cpp
    Transform.cpp
    ScopedTimer.cpp
)

target_link_libraries(Core
    PUBLIC
        linalg
)

target_include_directories(Core PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

