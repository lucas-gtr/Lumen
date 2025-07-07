function(target_enable_sanitizers TARGET)
  if(NOT TARGET ${TARGET})
    message(FATAL_ERROR "target_enable_sanitizers: target '${TARGET}' does not exist")
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(STATUS "Enabling sanitizers for target '${TARGET}' (GCC/Clang)")

    target_compile_options(${TARGET} PRIVATE
      -fsanitize=address,undefined,leak,float-divide-by-zero
      -fno-omit-frame-pointer
    )
    target_link_options(${TARGET} PRIVATE
      -fsanitize=address,undefined,leak,float-divide-by-zero
    )

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING "Sanitizers on MSVC are limited or experimental; skipping sanitizers for target '${TARGET}'")

  else()
    message(WARNING "Unsupported compiler '${CMAKE_CXX_COMPILER_ID}': sanitizers not enabled for target '${TARGET}'")
  endif()
endfunction()
