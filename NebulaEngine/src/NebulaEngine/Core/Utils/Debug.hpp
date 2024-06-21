#pragma once

#ifdef NEBULA_DEBUG
    #define NEBULA_RUN_IF_DEBUG(...) \
        switch (0)                   \
        {                            \
        default: {                   \
            \ __VA_ARGS__;           \
        }                            \
            \                        \
        }
#else
    #define NEBULA_RUN_IF_DEBUG(...)
#endif
