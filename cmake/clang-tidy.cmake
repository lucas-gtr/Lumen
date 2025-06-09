function(run_clang_tidy)
    find_program(CLANG_TIDY "clang-tidy")
    
    if(NOT CLANG_TIDY)
        message(WARNING "Clang-Tidy not found. Skipping static analysis.")
        return()
    endif()

    set(SOURCE_FILES "")
    
    if(FILES_TO_CHECK)
        if(FILES_TO_CHECK STREQUAL "all")
            file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
            list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")
        else()
            foreach(file IN LISTS FILES_TO_CHECK)
                if(file MATCHES ".*\\.cpp$" )
                    list(APPEND SOURCE_FILES ${file})
                endif()
            endforeach()
        endif()
    else()
        message(STATUS "No files specified for Clang-Tidy. No files will be checked.")
        return()
    endif()
    
    add_custom_target(run-clang-tidy
        COMMAND ${CLANG_TIDY} 
        --config-file=${CMAKE_SOURCE_DIR}/cmake/.clang-tidy
        --quiet
        ${SOURCE_FILES} 
        -p=${CMAKE_BINARY_DIR}
        -header-filter=${CMAKE_SOURCE_DIR}/include/.*
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Clang-Tidy"
        VERBATIM
    )
endfunction()