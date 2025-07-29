cmake_minimum_required(VERSION 3.10)

set(PROJECT_TARGET_NAME Lumen)

project(${PROJECT_TARGET_NAME}
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(external/linalg)

add_subdirectory(src/Diagnostics)
add_subdirectory(src/Core)
add_subdirectory(src/BVH)
add_subdirectory(src/Surface)
add_subdirectory(src/Geometry)
add_subdirectory(src/SceneObjects)
add_subdirectory(src/Lighting)
add_subdirectory(src/Scene)
add_subdirectory(src/Rendering)
add_subdirectory(src/Export)
add_subdirectory(src/GPU)
add_subdirectory(src/GUI)

add_executable(Lumen src/main.cpp)

target_include_directories(Lumen PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Lumen
    PRIVATE
        linalg
        Core
        BVH
        Geometry
        Surface
        SceneObjects
        Lighting
        Scene
        Rendering
        Export      
        GPU
        RenderingGL  
        GUI
        Diagnostics
)

option(ENABLE_WARNINGS             "Enable compiler warnings"                    ON)
option(ENABLE_SANITIZERS           "Enable runtime sanitizers"                   OFF)
option(ENABLE_LTO                  "Enable link-time optimization"               OFF)
option(ENABLE_CLANG_FORMAT         "Run clang-format in build"                   OFF)
option(ENABLE_CLANG_TIDY           "Run clang-tidy analysis"                     OFF)
option(ENABLE_FIX_CLANG_TIDY       "Run clang-tidy with --fix option"            OFF)
option(ENABLE_DOXYGEN              "Build documentation with Doxygen"            OFF)
option(ENABLE_OPTIMIZATIONS        "Enable high-performance compile flags"       OFF)
option(ENABLE_UNIT_TESTS           "Enable tests with GoogleTest"                OFF)

if(ENABLE_OPTIMIZATIONS AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_TARGET_NAME} PRIVATE -O3 -march=native)
endif()

if(ENABLE_OPTIMIZATIONS AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_TARGET_NAME} PRIVATE /O2)
endif()

if(ENABLE_WARNINGS)
    include(warnings)
    target_enable_warnings(${PROJECT_TARGET_NAME})
endif()

if(ENABLE_SANITIZERS)
    include(sanitizers)    
    target_enable_sanitizers(${PROJECT_TARGET_NAME})
endif()

if(ENABLE_LTO)
    include(lto)
    target_enable_lto(${PROJECT_TARGET_NAME})
endif()

if(ENABLE_CLANG_FORMAT)
    include(clang-format)
    run_clang_format()
endif()

if(ENABLE_CLANG_TIDY)
    include(clang-tidy)
    run_clang_tidy(${ENABLE_FIX_CLANG_TIDY})
endif()

if(ENABLE_DOXYGEN)
    include(Doxygen)
    generate_doxygen_documentation()
endif()

if(ENABLE_UNIT_TESTS)
    enable_testing()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g" CACHE STRING "Compiler flags for tests" FORCE)

    add_subdirectory(external/googletest)
    add_subdirectory(tests)
endif()
