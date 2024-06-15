#pragma once

#include <cstddef>
#include <gsl/gsl>
#include <memory>
#include <utility>
#include <variant>

#include "Log.hpp"

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
        template<typename U> friend class Ref;
        template<typename U> friend class Ptr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        Ref([[maybe_unused]] std::nullptr_t value) noexcept : m_Ptr(nullptr) {}

        explicit Ref(T& value) noexcept : m_Ptr(&value)
        {
#ifdef NEBULA_DEBUG
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_IsValue = true;
#endif
        }

        ~Ref()
        {
#ifdef NEBULA_DEBUG
            if (!m_IsValue)
            {
                if (m_Ptr != nullptr && m_RefCount.expired())
                {
                    NEBULA_CORE_LOG_WARN("Deleting a reference with no reference count");
                }
            }
#endif
        }

        Ref(const Ref& other) : m_Ptr(other.m_Ptr)
        {
#ifdef NEBULA_DEBUG
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_RefCount = other.m_RefCount;
#endif
        }

        Ref& operator=(const Ref& other)
        {
            if (this != &other)
            {
                m_Ptr = other.m_Ptr;
#ifdef NEBULA_DEBUG
                m_RefCount = other.m_RefCount;
                m_IsValue  = other.m_IsValue;
#endif
            }
            return *this;
        }

        Ref(Ref&& other) noexcept : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
#ifdef NEBULA_DEBUG
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_IsValue = other.m_IsValue;
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            m_RefCount = std::move(other.m_RefCount);
#endif
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if (this != &other)
            {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
#ifdef NEBULA_DEBUG
                // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
                m_IsValue = other.m_IsValue;
                // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
                m_RefCount = std::move(other.m_RefCount);
#endif
            }
            return *this;
        }

        /**
         * @brief Access the pointer
         * @return The pointer
         */
        T* operator->() const noexcept
        {
#ifdef NEBULA_DEBUG
            if (m_Ptr == nullptr)
            {
                NEBULA_CORE_LOG_WARN("Accessing a null pointer");
            }
            if (!m_IsValue)
            {
                if (m_RefCount.expired())
                {
                    NEBULA_CORE_LOG_WARN("Accessing a pointer with no reference count");
                }
            }
#endif
            return m_Ptr;
        }

        /**
         * @brief Dereference the pointer
         * @return The value of the pointer
         */
        T& operator*() const noexcept
        {
#ifdef NEBULA_DEBUG
            if (m_Ptr == nullptr)
            {
                NEBULA_CORE_LOG_WARN("Dereferencing a null pointer");
            }
            if (!m_IsValue)
            {
                if (m_RefCount.expired())
                {
                    NEBULA_CORE_LOG_WARN("Dereferencing a pointer with no reference count");
                }
            }
#endif
            return *m_Ptr;
        }

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        operator bool() const noexcept { return m_Ptr != nullptr; }

        bool operator==(const Ref<T>& other) const noexcept { return m_Ptr == other.m_Ptr; }

        bool operator!=(const Ref<T>& other) const noexcept { return m_Ptr != other.m_Ptr; }

        bool operator==(T* other) const noexcept { return m_Ptr == other; }

        bool operator!=(T* other) const noexcept { return m_Ptr != other; }

        template<typename R> Ref<R> StaticCast() noexcept
        {
            auto ref = Ref<R>(static_cast<R*>(m_Ptr));
#ifdef NEBULA_DEBUG
            ref.m_IsValue  = m_IsValue;
            ref.m_RefCount = m_RefCount;
#endif
            return ref;
        }

        template<typename R> Ref<R> DynamicCast() noexcept
        {
            auto ref = Ref<R>(dynamic_cast<R*>(m_Ptr));
#ifdef NEBULA_DEBUG
            ref.m_IsValue  = m_IsValue;
            ref.m_RefCount = m_RefCount;
#endif
            return ref;
        }
    private:
        T* m_Ptr;
#ifdef NEBULA_DEBUG
        std::weak_ptr<std::monostate> m_RefCount;
        bool m_IsValue = false; // Is a temporary value
#endif
    };

    /**
    * @brief A zero abstraction pointer, that shows clear ownership, and automatically deletes the pointer when it goes out of scope
    * @tparam T The type of the pointer
    */
    template<typename T> class Ptr
    {
    public:
        template<typename R> friend class Ptr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        Ptr(std::nullptr_t) noexcept : m_Ptr(nullptr) {}
        explicit Ptr(gsl::owner<T*> ptr) noexcept : m_Ptr(ptr) {}

        Ptr() noexcept : m_Ptr(nullptr) {}

        ~Ptr() { delete m_Ptr; }

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
            ref.m_RefCount = m_RefCount;
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
        T& operator*() const noexcept { return *m_Ptr; }

        T* Get() const noexcept
        {
            return m_Ptr; // NOLINT(cppcoreguidelines-owning-memory)
        }

        bool operator==(const Ref<T>& other) const noexcept { return m_Ptr == other.m_Ptr; }

        bool operator!=(const Ref<T>& other) const noexcept { return m_Ptr != other.m_Ptr; }

        bool operator==(T* other) const noexcept { return m_Ptr == other; }

        bool operator!=(T* other) const noexcept { return m_Ptr != other; }

        template<typename R> Ptr<R> Cast() noexcept
        {
            auto ptr = Ptr<R>(static_cast<R*>(m_Ptr));
#ifdef NEBULA_DEBUG
            ptr.m_RefCount = m_RefCount;
#endif
            m_Ptr = nullptr; // NOLINT(cppcoreguidelines-owning-memory)
            return ptr;
        }

        // No dynamic cast, because it might fail, and we don't want to delete the pointer

        void Reset() noexcept
        {
            delete m_Ptr;
            m_Ptr = nullptr; // NOLINT(cppcoreguidelines-owning-memory)
        }

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        operator bool() const noexcept { return m_Ptr != nullptr; }
    private:
        gsl::owner<T*> m_Ptr;
#ifdef NEBULA_DEBUG
        std::shared_ptr<std::monostate> m_RefCount = std::make_shared<std::monostate>();
#endif
    };

    template<typename T, typename... Args> Ptr<T> MakePtr(Args&&... args)
    {
        return Ptr<T>(new T(std::forward<Args>(args)...));
    }
} // namespace Nebula
