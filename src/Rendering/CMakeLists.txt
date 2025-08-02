add_library(Rendering STATIC
    Renderer.cpp
    SingleThreaded.cpp
    MultiThreadedCPU.cpp
    CameraRayEmitter.cpp
    PathTracer/RayIntersection.cpp
    RenderSettings.cpp
    RenderTime.cpp
    PathTracer/PathTracer.cpp
)

target_include_directories(Rendering PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Rendering PRIVATE Core Geometry SceneObjects Scene Surface)
