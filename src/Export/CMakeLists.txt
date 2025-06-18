add_library(Export STATIC
    RenderExporter.cpp
    OutputFormatPng.cpp
    OutputFormatJpeg.cpp
    OutputFormatTga.cpp
    OutputFormatBmp.cpp
    OutputFormatHdr.cpp
)

target_include_directories(Export PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
)
target_link_libraries(Export PRIVATE Core Rendering)
