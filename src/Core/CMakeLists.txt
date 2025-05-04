add_library(Core STATIC
    Framebuffer.cpp
    Transform.cpp
    Random.cpp
)

target_include_directories(Core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)
