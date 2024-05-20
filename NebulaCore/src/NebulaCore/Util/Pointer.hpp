#pragma once

#include <gsl/gsl>

#include "../Log/Log.hpp"

namespace Nebula
{
    template<typename T> class Ptr;

    /**
     * @brief A reference to a pointer, that does not own the pointer, and does not delete it when it goes out of scope
     * @tparam T The type of the pointer
     */
    template<typename T> class Ref
    {
    private:
        explicit Ref(T* ptr) noexcept : m_Ptr(ptr) {}
    public:
        friend class Ptr<T>;

        explicit Ref(T& value) noexcept : m_Ptr(&value) {}

        ~Ref()
        {
#ifdef NEBULA_DEBUG
            if (m_RefCount != nullptr)
            {
                if (*m_RefCount == 0)
                {
                    NEB_CORE_LOG_WARN("Deleting a reference with no references left");
                }
                else
                {
                    (*m_RefCount)--;
                }
            }
#endif
        }

        Ref(const Ref& other) : m_Ptr(other.m_Ptr)
        {
#ifdef NEBULA_DEBUG
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_RefCount = other.m_RefCount;
            (*m_RefCount)++;
#endif
        }

        Ref& operator=(const Ref& other)
        {
            if (this != &other)
            {
                m_Ptr = other.m_Ptr;
#ifdef NEBULA_DEBUG
                if (m_RefCount != nullptr)
                {
                    m_RefCount = other.m_RefCount;
                    (*m_RefCount)++;
                }
#endif
            }
            return *this;
        }

        Ref(Ref&& other) noexcept : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
#ifdef NEBULA_DEBUG
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_RefCount       = other.m_RefCount;
            other.m_RefCount = nullptr;
#endif
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if (this != &other)
            {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
#ifdef NEBULA_DEBUG
                m_RefCount       = other.m_RefCount;
                other.m_RefCount = nullptr;
#endif
            }
            return *this;
        }

        /**
         * @brief Access the pointer
         * @return The pointer
         */
        T* operator->() const noexcept { return m_Ptr; }

        /**
         * @brief Dereference the pointer
         * @return The value of the pointer
         */
        T operator*() const noexcept { return *m_Ptr; }
    private:
        T* m_Ptr;
#ifdef NEBULA_DEBUG
        std::size_t* m_RefCount = nullptr;
#endif
    };

    /**
    * @brief A zero abstraction pointer, that shows clear ownership, and automatically deletes the pointer when it goes out of scope
    * @tparam T The type of the pointer
    */
    template<typename T> class Ptr
    {
    public:
        explicit Ptr(gsl::owner<T*> ptr) noexcept : m_Ptr(ptr) {}

        ~Ptr()
        {
#ifdef NEBULA_DEBUG
            if (m_RefCount != 0)
            {
                NEB_CORE_LOG_WARN("Deleting a pointer with {0} references left", m_RefCount);
            }
#endif
            delete m_Ptr;
        }

        Ptr(const Ptr&)            = delete;
        Ptr& operator=(const Ptr&) = delete;

        Ptr(Ptr&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }

        Ptr& operator=(Ptr&& other) noexcept
        {
            if (this != &other)
            {
                delete m_Ptr;
                m_Ptr       = other.m_Ptr; // NOLINT(cppcoreguidelines-owning-memory)
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        /**
         * @brief Create a reference to the pointer
         * @return The reference
         */
        Ref<T> GetRef() noexcept
        {
#ifdef NEBULA_DEBUG
            auto ref       = Ref<T>(m_Ptr);
            ref.m_RefCount = &m_RefCount;
            m_RefCount++;
            return ref;
#else
            return Ref<T>(m_Ptr);
#endif
        }

        T* operator->() const noexcept
        {
            return m_Ptr; // NOLINT(cppcoreguidelines-owning-memory)
        }

        /**
        * @brief Dereference the pointer
        * @return The value of the pointer
        **/
        T operator*() const noexcept { return *m_Ptr; }

        T* Get() const noexcept
        {
            return m_Ptr; // NOLINT(cppcoreguidelines-owning-memory)
        }
    private:
        gsl::owner<T*> m_Ptr;
#ifdef NEBULA_DEBUG
        std::size_t m_RefCount = 0;
#endif
    };
} // namespace Nebula
