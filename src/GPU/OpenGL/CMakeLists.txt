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

find_package(Qt6 REQUIRED COMPONENTS OpenGL OpenGLWidgets)

# OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

target_link_libraries(RenderingGL PUBLIC Qt6::OpenGL Qt6::OpenGLWidgets GPU OpenGL::GL Geometry)

target_include_directories(RenderingGL PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)