add_library(Surface STATIC
    Texture.cpp
    TextureFiltering.cpp
    TextureWrapping.cpp
    Material.cpp
    TextureLoader.cpp
    TextureManager.cpp
    MaterialManager.cpp
)

target_include_directories(Surface PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(Surface PRIVATE Core)
