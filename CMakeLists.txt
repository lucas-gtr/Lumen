cmake_minimum_required(VERSION 3.10)

project(Lumen VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_library(Factorial STATIC src/factorial.cpp)
target_include_directories(Factorial PUBLIC
    ${CMAKE_SOURCE_DIR}/include 
)

add_executable(Lumen src/main.cpp)

target_include_directories(Lumen PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(Lumen PUBLIC
    Factorial    
    gtest
)

option(ENABLE_COMPILE_WARNINGS "Enable Compiler Warnings" ON)
option(ENABLE_SANITIZERS "Enable Sanitizers" ON)
option(ENABLE_LTO "Enable Link Time Optimization" ON)

option(ENABLE_CLANG_FORMAT "Enable Clang-Format" OFF)
option(ENABLE_CLANG_TIDY "Enable Clang-Tidy" OFF)
option(ENABLE_DOXYGEN "Enable Doxygen" OFF)
option(ENABLE_TESTS "Enable Tests" OFF)

set(FILES_TO_CHECK "" CACHE STRING "Files to check with Clang-Tidy and Clang-Format. Set to 'all' to check all source files.")

if(ENABLE_CLANG_FORMAT)
    include(clang-format)
    run_clang_format()
endif()

if(ENABLE_CLANG_TIDY)
    include(clang-tidy)
    run_clang_tidy()
endif()

if(ENABLE_COMPILE_WARNINGS)
    include(warnings)
    check_compile_warnings(Lumen)
endif()

if(ENABLE_SANITIZERS)
    include(sanitizers)    
    add_sanitizers()
endif()

if(ENABLE_LTO)
    include(lto)
    enable_lto(Lumen)
endif()

if(ENABLE_DOXYGEN)
    include(Doxygen)
    generate_doxygen_docs()
endif()

if(ENABLE_TESTS)
    add_subdirectory(external/googletest)
    add_subdirectory(tests)
endif()