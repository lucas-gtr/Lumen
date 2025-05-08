add_library(ImplementationParameters STATIC
    Parameters.cpp
)

target_include_directories(ImplementationParameters PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)