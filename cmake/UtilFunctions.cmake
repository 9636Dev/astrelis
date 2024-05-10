function(nebula_add_definitions target define_prefix type)
    # If it is shared
    if(${type} STREQUAL "SHARED")
        target_compile_definitions(${target} PRIVATE NEBULA_EXPORT_DLL)
        target_compile_definitions(${target} PUBLIC ${define_prefix}_SHARED)
    else()
        target_compile_definitions(${target} PUBLIC ${define_prefix}_STATIC)
    endif()
endfunction()

function(nebula_library_type type output)
    if (${type} STREQUAL "Inherit")
        if (${NEBULA_LIBRARY_TYPE} STREQUAL "Shared")
                set(${output} SHARED PARENT_SCOPE)
        elseif (${NEBULA_LIBRARY_TYPE} STREQUAL "Static")
            set(${output} STATIC PARENT_SCOPE)
        endif()
    elseif (${type} STREQUAL "Shared")
        set(${output} SHARED PARENT_SCOPE)
    elseif (${type} STREQUAL "Static")
        set(${output} STATIC PARENT_SCOPE)
    endif()
endfunction()

function(nebula_format_project target dir sources)
    if (NEBULA_ENABLE_CLANG_FORMAT)
        set(format_target ${target}_format)

        add_custom_target(${format_target}
            COMMAND ${CLANG_FORMAT} -i -style=file ${sources}
            WORKING_DIRECTORY ${dir}
        )
    endif()
endfunction()
