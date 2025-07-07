add_library(GPU STATIC
  IObjectGPU.cpp
  ITextureGPU.cpp
  Lights/DirectionalGPU.cpp
  Lights/PointGPU.cpp
  Lights/SpotGPU.cpp
)

target_include_directories(GPU PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(GPU PUBLIC Core Geometry Lighting Surface)

add_subdirectory(OpenGL)