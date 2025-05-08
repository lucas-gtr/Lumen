add_library(Scene STATIC
    Scene.cpp
    Skybox.cpp
)

target_include_directories(Scene PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(Scene PRIVATE Core BVH Geometry Surface SceneObjects Lighting)
