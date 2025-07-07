function(run_clang_tidy FIX_LINT)
    find_program(CLANG_TIDY_EXECUTABLE "clang-tidy")

    if(NOT CLANG_TIDY_EXECUTABLE)
        message(WARNING "clang-tidy not found. Skipping static analysis.")
        return()
    endif()

    message(STATUS "Files to check with clang-tidy: ${FILES_TO_CHECK}")

    if(NOT DEFINED FILES_TO_CHECK)
        set(FILES_TO_CHECK "" CACHE STRING "Files to check with clang-tidy (space separated), empty for default")
    endif()

    set(CLANG_TIDY_TARGET_FILES)

    if(FILES_TO_CHECK STREQUAL "")
        file(GLOB_RECURSE src_cpp_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
        list(FILTER src_cpp_files EXCLUDE REGEX ".*/main.cpp$")
        list(APPEND CLANG_TIDY_TARGET_FILES
          ${src_cpp_files}
      )
    else()
        separate_arguments(FILES_TO_CHECK)
        foreach(file IN LISTS FILES_TO_CHECK)
            if(file MATCHES ".*\\.cpp$")
                if(NOT IS_ABSOLUTE "${file}")
                    get_filename_component(abs_file "${CMAKE_SOURCE_DIR}/${file}" ABSOLUTE)
                else()
                    set(abs_file "${file}")
                endif()
                if(EXISTS "${abs_file}")
                    list(APPEND CLANG_TIDY_TARGET_FILES "${abs_file}")
                else()
                    message(WARNING "Specified file '${file}' does not exist. Skipping.")
                endif()
            else()
                message(WARNING "Specified file '${file}' is not a .cpp file. Skipping.")
            endif()
        endforeach()
    endif()

    list(REMOVE_DUPLICATES CLANG_TIDY_TARGET_FILES)

    list(LENGTH CLANG_TIDY_TARGET_FILES num_files)
    if(num_files EQUAL 0)
        add_custom_target(run-clang-tidy
            COMMAND ${CMAKE_COMMAND} -E echo "No source files found for clang-tidy. Skipping."
            COMMENT "No source files to lint."
            VERBATIM
        )
        return()
    endif()

    string(JOIN ";" CLANG_TIDY_FILE_ARGS ${CLANG_TIDY_TARGET_FILES})

    set(CLANG_TIDY_ARGS
        --config-file=${CMAKE_SOURCE_DIR}/cmake/.clang-tidy
        --quiet
        -p=${CMAKE_BINARY_DIR}
        -header-filter=${CMAKE_SOURCE_DIR}/include/.*
        ${CLANG_TIDY_FILE_ARGS}
    )

    if(FIX_LINT)
        list(APPEND CLANG_TIDY_ARGS --fix)
    endif()

    add_custom_target(run-clang-tidy
        COMMAND ${CLANG_TIDY_EXECUTABLE} ${CLANG_TIDY_ARGS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy static analysis"
        VERBATIM
    )
endfunction()
