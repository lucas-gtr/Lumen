add_library(ImplementationParameters STATIC
    Parameters.cpp
)

target_include_directories(ImplementationParameters PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)