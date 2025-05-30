add_library(BVH STATIC
    BVHBuilder.cpp
    BVHNode.cpp
)

target_include_directories(BVH PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)