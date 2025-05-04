add_library(Geometry STATIC
    Mesh.cpp
    CubeMeshBuilder.cpp
    SphereMeshBuilder.cpp
    PlaneMeshBuilder.cpp
    OBJLoader.cpp
)

target_include_directories(Geometry PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/eigen
)

target_link_libraries(Geometry PUBLIC Core)
