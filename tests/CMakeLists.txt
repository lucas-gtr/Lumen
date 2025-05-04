add_executable(Tests tests.cpp)

target_sources(Tests PRIVATE 
    Scene/CameraTests.cpp
    Geometry/MeshTests.cpp
    Geometry/CubeMeshBuilderTests.cpp
    Geometry/PlaneMeshBuilderTests.cpp
    Geometry/SphereMeshBuilderTests.cpp
    Geometry/OBJLoaderTests.cpp
    Core/RayTests.cpp
    Core/FramebufferTests.cpp
    Core/ColorUtilsTests.cpp
    Scene/SceneTests.cpp
    Scene/Object3DTests.cpp
    Scene/SkyboxTests.cpp
    Core/TransformTests.cpp
    Rendering/RayIntersectionTests.cpp
    Core/RandomTests.cpp
    PostProcessing/ExposureTests.cpp
    PostProcessing/NoneTests.cpp
    PostProcessing/ReinhardTests.cpp
    Rendering/RenderSettingsTests.cpp
    Rendering/CameraRayEmitterTests.cpp
    Rendering/RendererTests.cpp
    Export/RenderExporterTests.cpp
    Export/OutputFormatPngTests.cpp
    Lighting/LightTests.cpp
    Lighting/DirectionalLightTests.cpp
    Lighting/PointLightTests.cpp
    Lighting/SpotLightTests.cpp
    ImplementationParameters/ParametersTests.cpp
    ImplementationParameters/ParameterTypesTests.cpp
    Surface/MaterialTests.cpp
    Surface/TextureTests.cpp
    Surface/TextureLoaderTests.cpp
    Surface/TextureFilteringTests.cpp
    Surface/TextureWrappingTests.cpp
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
