function(verify_variable var)
    set(value ${${var}})
    set(valid_values ${ARGN})
    list(FIND valid_values "${value}" index)
    if(index EQUAL -1)
        message(FATAL_ERROR "Invalid value for ${var}: ${value}")
        message(FATAL_ERROR "Valid values are: ${valid_values}")
    endif()
endfunction()

# Verify that the variables are set to valid values
verify_variable(NEBULA_LIBRARY_TYPE "Static" "Shared")
verify_variable(NEBULA_BUILD_TYPE "Debug" "Release")
