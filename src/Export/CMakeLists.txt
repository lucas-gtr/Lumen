add_library(Export STATIC
    RenderExporter.cpp
    OutputFormatPng.cpp
)

target_include_directories(Export PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
    ${CMAKE_SOURCE_DIR}/external/stb
)
target_link_libraries(Export PRIVATE Core Rendering ImplementationParameters)
