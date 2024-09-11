#include "Profiling.hpp"

#ifdef TRACY_ENABLE
    #ifdef ASTRELIS_PROFILE_MEMORY
void* operator new(size_t size)
{
    // NOLINTNEXTLINE
    void* ptr = malloc(size);
    TracyAlloc(ptr, size);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    // NOLINTNEXTLINE
    free(ptr);
}
    #endif
#endif
