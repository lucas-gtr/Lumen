add_library(PostProcessing STATIC
    ToneMapping.cpp
)

target_include_directories(PostProcessing PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(PostProcessing PRIVATE Core)
