#include "Profiling.hpp"

#ifdef TRACY_ENABLE
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
