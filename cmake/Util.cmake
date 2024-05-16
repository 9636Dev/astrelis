function(nebula_target target)
    target_compile_options(${target} PUBLIC "${NEBULA_COMPILER_FLAGS}")
endfunction()
