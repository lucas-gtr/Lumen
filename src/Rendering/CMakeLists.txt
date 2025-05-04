add_library(Rendering STATIC
    Renderer.cpp
    CameraRayEmitter.cpp
    RayIntersection.cpp
    RenderSettings.cpp
)

target_include_directories(Rendering PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(Rendering PUBLIC Core Scene Geometry)
