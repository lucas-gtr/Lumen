function(generate_doxygen_documentation)
    find_package(Doxygen)
    
    if (NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found, documentation target will not be available.")
        return()
    endif()
    
    set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs")
    set(DOXYGEN_CONFIG_IN "${CMAKE_SOURCE_DIR}/docs/Doxyfile.in" CACHE FILEPATH "Doxygen config template")
    set(DOXYGEN_CONFIG_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

    file(MAKE_DIRECTORY "${DOXYGEN_OUTPUT_DIR}")

    configure_file(${DOXYGEN_CONFIG_IN} ${DOXYGEN_CONFIG_OUT} @ONLY)

    add_custom_target(generate-doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
endfunction()
