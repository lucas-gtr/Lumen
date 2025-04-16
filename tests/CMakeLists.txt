add_executable(Tests tests.cpp)

target_sources(Tests PRIVATE 
    Core/CameraTests.cpp
    Core/MeshTests.cpp
    Core/RayTests.cpp
    Core/FramebufferTests.cpp
    Core/SceneTests.cpp
    Core/Object3DTests.cpp
    Core/TransformTests.cpp
    Core/RayIntersectionTests.cpp
    Core/RandomTests.cpp
    Rendering/RenderSettingsTests.cpp
    Rendering/CameraRayEmitterTests.cpp
    Rendering/RendererTests.cpp
    Export/RenderExporterTests.cpp
    Export/OutputFormatPngTests.cpp
)

target_link_libraries(Tests PUBLIC 
    Core
    Export
    Rendering
    gtest
    gmock)

target_include_directories(Tests PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/googletest/include
    ${CMAKE_SOURCE_DIR}/external/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)
