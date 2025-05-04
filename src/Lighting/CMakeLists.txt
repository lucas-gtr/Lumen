add_library(Lighting STATIC
    DirectionalLight.cpp
    PointLight.cpp
    SpotLight.cpp
)

target_include_directories(Lighting PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(Lighting PUBLIC Core)
