function(run_clang_format)
    find_program(CLANG_FORMAT "clang-format")
    
    if(NOT CLANG_FORMAT)
        message(WARNING "Clang-Format not found. Skipping formatting.")
        return()
    endif()
    
    set(SOURCE_FILES "")
    
    if(FILES_TO_CHECK)
        if(FILES_TO_CHECK STREQUAL "all")
            file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/include/*.hpp")
        else()
            foreach(file IN LISTS FILES_TO_CHECK)
                if(file MATCHES ".*\\.cpp$" OR file MATCHES ".*\\.h$" OR file MATCHES ".*\\.hpp$")
                    list(APPEND SOURCE_FILES ${file})
                endif()
            endforeach()
        endif()
    else()
        message(STATUS "No files specified for Clang-Format. No files will be checked.")
        return()
    endif()
    
    add_custom_target(run-clang-format
        COMMAND ${CLANG_FORMAT} 
        -i
        -verbose
        --style=file:${CMAKE_SOURCE_DIR}/cmake/.clang-format
        --fallback-style=none
        ${SOURCE_FILES}
        COMMENT "Running Clang-Format"
        VERBATIM
    )
endfunction()