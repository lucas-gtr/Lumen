add_library(RenderingGL STATIC
    UniformBufferGL.cpp
    FramebufferGL.cpp
    ShadersGL.cpp
    TextureGL.cpp
    MaterialGL.cpp
    CameraGL.cpp
    ObjectGL.cpp
    SkyboxGL.cpp
    Lights/PointGL.cpp
    Lights/SpotGL.cpp
    Lights/DirectionalGL.cpp
    Lights/ShadowMapGL.cpp
    ResourceManagerGL.cpp
    EngineGL.cpp
)

# GLEW 
find_package(GLEW REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS OpenGL OpenGLWidgets)

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

target_link_libraries(RenderingGL PUBLIC Qt6::OpenGL Qt6::OpenGLWidgets GPU GLEW::GLEW glfw OpenGL::GL Geometry)

target_include_directories(RenderingGL PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
)