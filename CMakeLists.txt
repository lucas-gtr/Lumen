cmake_minimum_required(VERSION 3.10)

project(Lumen VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

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
add_subdirectory(src/Qt)

add_executable(Lumen src/main.cpp)

target_include_directories(Lumen PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(Lumen
    PRIVATE
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
        QtInterface
)

option(ENABLE_COMPILE_WARNINGS "Enable Compiler Warnings" ON)
option(ENABLE_DEBUG_COMPILE "Enable Debug Compile" ON)
option(ENABLE_SANITIZERS "Enable Sanitizers" ON)
option(ENABLE_COMPILER_OPTIMIZATIONS "Enable Compiler Optimizations" ON)
option(ENABLE_LTO "Enable Link Time Optimization" OFF)

option(ENABLE_CLANG_FORMAT "Enable Clang-Format" OFF)
option(ENABLE_CLANG_TIDY "Enable Clang-Tidy" OFF)
option(ENABLE_DOXYGEN "Enable Doxygen" OFF)
option(ENABLE_TESTS "Enable Tests" OFF)

set(FILES_TO_CHECK "" CACHE STRING "Files to check with Clang-Tidy and Clang-Format. Set to 'all' to check all source files.")

if(ENABLE_COMPILER_OPTIMIZATIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native" CACHE STRING "Compiler flags for optimization" FORCE)
endif()

if(ENABLE_DEBUG_COMPILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0" CACHE STRING "Compiler flags for debug" FORCE)
endif()

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g" CACHE STRING "Compiler flags for tests" FORCE)
    add_subdirectory(external/googletest)
    add_subdirectory(tests)
endif()
