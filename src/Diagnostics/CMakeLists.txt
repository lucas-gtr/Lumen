add_library(Diagnostics STATIC
    ScopedTimer.cpp
)

target_include_directories(Diagnostics PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

