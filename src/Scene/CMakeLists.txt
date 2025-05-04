add_library(Scene STATIC
    Camera.cpp
    Object3D.cpp
    Scene.cpp
    Skybox.cpp
)

target_include_directories(Scene PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Scene PUBLIC Core Lighting Surface)
