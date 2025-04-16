add_library(Core STATIC
    Framebuffer.cpp
    RayIntersection.cpp
    Transform.cpp
    Camera.cpp
    Mesh.cpp
    Random.cpp
)

target_include_directories(Core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)
