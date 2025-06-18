add_executable(Tests tests.cpp)

target_sources(Tests PRIVATE 
    BVH/BVHBuilderTests.cpp
    BVH/BVHNodeTests.cpp
    Core/CommonTypesTests.cpp
    Core/ColorUtilsTests.cpp
    Core/FramebufferTests.cpp
    Core/RandomTests.cpp
    Core/RayTests.cpp
    Core/TransformTests.cpp
    Core/Math/Vec2Tests.cpp
    Core/Math/Vec3Tests.cpp
    Core/Math/Vec4Tests.cpp
    Core/Math/Mat3Tests.cpp
    Core/Math/Mat4Tests.cpp
    Core/Math/linTests.cpp
    Export/OutputFormatPngTests.cpp
    Export/RenderExporterTests.cpp
    Geometry/CubeMeshBuilderTests.cpp
    Geometry/MeshTests.cpp
    Geometry/OBJLoaderTests.cpp
    Geometry/PlaneMeshBuilderTests.cpp
    Geometry/SphereMeshBuilderTests.cpp
    GPU/IObjectGPUTests.cpp
    GPU/ITextureGPUTests.cpp
    GPU/IEngineGPUTests.cpp
    GPU/IResourceManagerGPUTests.cpp
    GPU/Lights/DirectionalGPUTests.cpp
    GPU/Lights/PointGPUTests.cpp
    GPU/Lights/SpotGPUTests.cpp
    GPU/OpenGL/DirectionalGLTests.cpp
    GPU/OpenGL/PointGLTests.cpp
    GPU/OpenGL/SpotGLTests.cpp
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
    BVH
    Geometry
    Surface
    SceneObjects
    Lighting
    Scene
    Rendering
    Export
    GPU
    RenderingGL
    QtInterface
    gtest
    gmock)

target_include_directories(Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/googletest/include
    ${CMAKE_SOURCE_DIR}/external/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/external/stb
)
