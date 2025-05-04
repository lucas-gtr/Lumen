add_library(Surface STATIC
    Texture.cpp
    TextureFiltering.cpp
    TextureWrapping.cpp
    Material.cpp
    TextureLoader.cpp
)

target_include_directories(Surface PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
    ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(Surface PUBLIC Core)
