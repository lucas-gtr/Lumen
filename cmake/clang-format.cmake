function(run_clang_format)
    find_program(CLANG_FORMAT "clang-format")
    
    if(NOT CLANG_FORMAT)
        message(WARNING "Clang-Format not found. Skipping formatting.")
        return()
    endif()
    
    if(NOT DEFINED FILES_TO_CHECK)
        set(FILES_TO_CHECK "" CACHE STRING "Files to check with clang-format (space separated), empty for default")
    endif()

    set(CLANG_FORMAT_TARGET_FILES)

    if(FILES_TO_CHECK STREQUAL "")
      file(GLOB_RECURSE src_cpp_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
      file(GLOB_RECURSE include_hpp_files "${CMAKE_SOURCE_DIR}/include/*.hpp")

      list(APPEND CLANG_FORMAT_TARGET_FILES
          ${src_cpp_files}
          ${include_hpp_files}
      )
    else()
        separate_arguments(FILES_TO_CHECK)
        foreach(file IN LISTS FILES_TO_CHECK)
            if(file MATCHES ".*\\.cpp$" OR file MATCHES ".*\\.hpp$")
                if(NOT IS_ABSOLUTE "${file}")
                    get_filename_component(abs_file "${CMAKE_SOURCE_DIR}/${file}" ABSOLUTE)
                else()
                    set(abs_file "${file}")
                endif()
                if(EXISTS "${abs_file}")
                    list(APPEND CLANG_FORMAT_TARGET_FILES "${abs_file}")
                else()
                    message(WARNING "Specified file '${file}' does not exist. Skipping.")
                endif()
            else()
                message(WARNING "Specified file '${file}' extension is not .cpp or .hpp. Skipping.")
            endif()
        endforeach()
    endif()

    list(REMOVE_DUPLICATES CLANG_FORMAT_TARGET_FILES)

    if(CLANG_FORMAT_TARGET_FILES STREQUAL "")
        message(STATUS "No source files found for clang-format.")
        return()
    endif()

    string(JOIN ";" CLANG_FORMAT_FILE_ARGS ${CLANG_FORMAT_TARGET_FILES})
    
    add_custom_target(run-clang-format
        COMMAND ${CLANG_FORMAT} 
        -i
        -verbose
        --style=file:${CMAKE_SOURCE_DIR}/cmake/.clang-format
        --fallback-style=none
        ${CLANG_FORMAT_TARGET_FILES}
        COMMENT "Running Clang-Format"
        VERBATIM
    )
endfunction()