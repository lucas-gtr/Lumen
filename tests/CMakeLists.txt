add_executable(Tests tests.cpp)

target_sources(Tests PRIVATE 
    BVH/BVHBuilderTests.cpp
    BVH/BVHNodeTests.cpp
    Core/ColorUtilsTests.cpp
    Core/FramebufferTests.cpp
    Core/RandomTests.cpp
    Core/RayTests.cpp
    Core/TransformTests.cpp
    Export/OutputFormatPngTests.cpp
    Export/RenderExporterTests.cpp
    Geometry/CubeMeshBuilderTests.cpp
    Geometry/MeshTests.cpp
    Geometry/OBJLoaderTests.cpp
    Geometry/PlaneMeshBuilderTests.cpp
    Geometry/SphereMeshBuilderTests.cpp
    ImplementationParameters/ParameterTypesTests.cpp
    ImplementationParameters/ParametersTests.cpp
    Lighting/DirectionalLightTests.cpp
    Lighting/LightTests.cpp
    Lighting/PointLightTests.cpp
    Lighting/SpotLightTests.cpp
    PostProcessing/ExposureTests.cpp
    PostProcessing/NoneTests.cpp
    PostProcessing/ReinhardTests.cpp
    Rendering/CameraRayEmitterTests.cpp
    Rendering/RayIntersectionTests.cpp
    Rendering/RenderSettingsTests.cpp
    Rendering/RendererTests.cpp
    Scene/SceneTests.cpp
    Scene/SkyboxTests.cpp
    SceneObjects/CameraTests.cpp
    SceneObjects/Object3DTests.cpp
    Surface/MaterialTests.cpp
    Surface/TextureFilteringTests.cpp
    Surface/TextureLoaderTests.cpp
    Surface/TextureTests.cpp
    Surface/TextureWrappingTests.cpp
)

target_link_libraries(Tests PRIVATE 
    Core
    ImplementationParameters
    BVH
    Geometry
    Surface
    SceneObjects
    Lighting
    Scene
    Rendering
    Export   
    gtest
    gmock)

target_include_directories(Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/googletest/include
    ${CMAKE_SOURCE_DIR}/external/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/external/eigen
    ${CMAKE_SOURCE_DIR}/external/stb
)
