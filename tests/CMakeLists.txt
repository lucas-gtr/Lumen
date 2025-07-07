add_executable(UnitTests main_tests.cpp)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**/*Tests.cpp")

target_sources(UnitTests PRIVATE ${TEST_SOURCES})

target_link_libraries(UnitTests PRIVATE 
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

target_include_directories(UnitTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/googletest/include
    ${CMAKE_SOURCE_DIR}/external/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/external/stb
)

add_test(NAME UnitTests COMMAND UnitTests)
