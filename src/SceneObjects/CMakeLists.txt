add_library(SceneObjects STATIC
    Camera.cpp
    Object3D.cpp
)

target_include_directories(SceneObjects PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(SceneObjects PRIVATE Core Geometry Surface)
