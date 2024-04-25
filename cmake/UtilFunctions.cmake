function(nebula_add_definitions target define_prefix type)
    # If it is shared
    if(${type} STREQUAL "Shared")
        target_compile_definitions(${target} PRIVATE NEBULA_EXPORT_DLL)
        target_compile_definitions(${target} PUBLIC ${define_prefix}_SHARED)
    else()
        target_compile_definitions(${target} PUBLIC ${define_prefix}_STATIC)
    endif()
endfunction()

