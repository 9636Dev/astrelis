if (NEBULA_ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT clang-format)
    if (NOT CLANG_FORMAT)
        message(FATAL_ERROR "clang-format not found")
    endif()
endif()

function(nebula_format_project target dir sources)
    if (NEBULA_ENABLE_CLANG_FORMAT)
        set(format_target ${target}_Format)

        add_custom_target(${format_target}
            COMMAND ${CLANG_FORMAT} -i -style=file ${sources}
            WORKING_DIRECTORY ${dir}
        )
    endif()
endfunction()

function(nebula_format_depends target dependency)
    if (NEBULA_ENABLE_CLANG_FORMAT)
        add_dependencies(${target}_Format ${dependency}_Format)
    endif()
endfunction()
