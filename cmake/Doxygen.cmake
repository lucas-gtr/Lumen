function(generate_doxygen_docs)
    find_package(Doxygen)
    
    if (NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found, documentation target will not be available.")
        return()
    endif()
    
    set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs")
    set(DOXYGEN_CONFIG_FILE "${CMAKE_BINARY_DIR}/Doxyfile")

    configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${DOXYGEN_CONFIG_FILE} @ONLY)

    add_custom_target(generate-doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endfunction()
