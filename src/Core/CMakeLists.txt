add_library(Core STATIC
    CommonTypes.cpp
    Framebuffer.cpp
    Transform.cpp
    Random.cpp
)

target_link_libraries(Core
    PUBLIC
        linalg
)

target_include_directories(Core PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

