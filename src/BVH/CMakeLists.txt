add_library(BVH STATIC
    BVHBuilder.cpp
    BVHNode.cpp
)

target_link_libraries(BVH
    PRIVATE
        linalg
)

target_include_directories(BVH PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)