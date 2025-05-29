add_library(Rendering STATIC
    Renderer.cpp
    SingleThreadedStrategy.cpp
    MultiThreadedCPUStrategy.cpp
    CameraRayEmitter.cpp
    RayIntersection.cpp
    RenderSettings.cpp
)

target_include_directories(Rendering PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(Rendering PRIVATE Core Geometry SceneObjects Scene Surface)
