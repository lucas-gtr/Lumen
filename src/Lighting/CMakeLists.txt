add_library(Lighting STATIC
    DirectionalLight.cpp
    PointLight.cpp
    SpotLight.cpp
)

target_include_directories(Lighting PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Lighting PRIVATE Core)
