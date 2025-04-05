add_executable(Tests tests.cpp)

target_link_libraries(Tests PUBLIC 
    gtest)

target_include_directories(Tests PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/googletest/include
)
