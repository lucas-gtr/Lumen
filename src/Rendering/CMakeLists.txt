add_library(Rendering STATIC
    Renderer.cpp
    SingleThreaded.cpp
    MultiThreadedCPU.cpp
    CameraRayEmitter.cpp
    RayIntersection.cpp
    RenderSettings.cpp
)

target_include_directories(Rendering PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Rendering PRIVATE Core Geometry SceneObjects Scene Surface)
