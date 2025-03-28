#include "Profiling.hpp"

#ifdef TRACY_ENABLE
    #ifdef ASTRELIS_PROFILE_MEMORY
/// @private
void* operator new(size_t size) {
    // NOLINTNEXTLINE
    void* ptr = malloc(size);
    TracyAlloc(ptr, size);
    return ptr;
}

/// @private
void operator delete(void* ptr) noexcept {
    TracyFree(ptr);
    // NOLINTNEXTLINE
    free(ptr);
}
    #endif
#endif
