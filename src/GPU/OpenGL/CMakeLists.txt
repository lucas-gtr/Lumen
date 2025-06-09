add_library(OpenGL STATIC
    WindowGL.cpp
    UniformBufferGL.cpp
    FramebufferGL.cpp
    ShadersGL.cpp
    TextureGL.cpp
    MaterialGL.cpp
    CameraGL.cpp
    ObjectGL.cpp
    Lights/PointGL.cpp
    Lights/SpotGL.cpp
    Lights/DirectionalGL.cpp
    Lights/ShadowMapGL.cpp
    ResourceManagerGL.cpp
    EngineGL.cpp
)

# GLEW 
find_package(GLEW REQUIRED)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Disable examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Disable tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Disable docs")
set(GLFW_BUILD_WAYLAND OFF CACHE INTERNAL "Disable Wayland support")
set(GLFW_BUILD_X11 ON CACHE INTERNAL "Enable X11 support")
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw ${CMAKE_BINARY_DIR}/glfw)

# OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

target_include_directories(OpenGL PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(OpenGL PRIVATE GPU GLEW::GLEW glfw OpenGL::GL Geometry)
