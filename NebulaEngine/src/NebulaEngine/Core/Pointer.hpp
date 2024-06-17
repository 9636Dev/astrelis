#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace Nebula
{
    // Forward declarations
    template<typename T> class ScopedPtr;
    template<typename T> class RefPtr;
    template<typename T> class OwnedPtr;
    template<typename T> class RawRef;

    template<typename T> class ScopedPtr
    {
    public:
        static_assert(!std::is_array_v<T>, "ScopedPtr does not support arrays!");
        static_assert(!std::is_reference_v<T>, "ScopedPtr does not support references!");
        template<typename U> friend class ScopedPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        ScopedPtr(std::nullptr_t) : m_Ptr(nullptr) {}

        explicit ScopedPtr(T* ptr) : m_Ptr(ptr) {}

        ~ScopedPtr() { delete m_Ptr; }

        ScopedPtr(const ScopedPtr&)            = delete;
        ScopedPtr& operator=(const ScopedPtr&) = delete;

        ScopedPtr(ScopedPtr&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }

        ScopedPtr& operator=(ScopedPtr&& other) noexcept
        {
            if (this != &other)
            {
                delete m_Ptr;
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        T* operator->() const noexcept { return m_Ptr; }

        T* Get() const noexcept { return m_Ptr; }

        void Reset() { delete m_Ptr; m_Ptr = nullptr; }

        template<typename U>
            requires std::is_base_of_v<U, T>
        /* NOLINT(google-explicit-constructor, hicpp-explicit-conversions) */ operator ScopedPtr<U>() const
        {
            return ScopedPtr<U>(static_cast<U*>(m_Ptr));
        }

        template<typename... Args> static ScopedPtr<T> Create(Args&&... args)
        {
            return ScopedPtr<T>(new T(std::forward<Args>(args)...));
        }
    private:
        T* m_Ptr;
    };

    using RefCountType = std::size_t;

    template<typename T> class RefPtr
    {
    public:
        static_assert(!std::is_array_v<T>, "RefPtr does not support arrays!");
        static_assert(!std::is_reference_v<T>, "RefPtr does not support references!");
        template<typename U> friend class RefPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        RefPtr(std::nullptr_t) : m_Ptr(nullptr), m_RefCount(nullptr) {}

        explicit RefPtr(T* ptr) : m_Ptr(ptr), m_RefCount(new RefCountType(1)) {}

        ~RefPtr()
        {
            if (m_RefCount != nullptr && --(*m_RefCount) == 0)
            {
                delete m_Ptr;
                delete m_RefCount;
            }
        }

        RefPtr(const RefPtr& other) : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
        {
            if (m_RefCount != nullptr)
            {
                ++(*m_RefCount);
            }
        }

        RefPtr& operator=(const RefPtr& other)
        {
            if (this != &other)
            {
                if (m_RefCount != nullptr && --(*m_RefCount) == 0)
                {
                    delete m_Ptr;
                    delete m_RefCount;
                }
                m_Ptr      = other.m_Ptr;
                m_RefCount = other.m_RefCount;
                if (m_RefCount != nullptr)
                {
                    ++(*m_RefCount);
                }
            }
            return *this;
        }

        RefPtr(RefPtr&& other) noexcept : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
        {
            other.m_Ptr      = nullptr;
            other.m_RefCount = nullptr;
        }

        RefPtr& operator=(RefPtr&& other) noexcept
        {
            if (this != &other)
            {
                if (m_RefCount != nullptr && --(*m_RefCount) == 0)
                {
                    delete m_Ptr;
                    delete m_RefCount;
                }
                m_Ptr            = other.m_Ptr;
                m_RefCount       = other.m_RefCount;
                other.m_Ptr      = nullptr;
                other.m_RefCount = nullptr;
            }
            return *this;
        }

        T* operator->() const noexcept { return m_Ptr; }

        void Reset()
        {
            if (m_RefCount != nullptr && --(*m_RefCount) == 0)
            {
                delete m_Ptr;
                delete m_RefCount;
            }
            m_Ptr      = nullptr;
            m_RefCount = nullptr;
        }

        template<typename U>
            requires std::is_base_of_v<U, T>
        /* NOLINT(google-explicit-constructor, hicpp-explicit-conversions) */ operator RefPtr<U>() const
        {
            return RefPtr<U>(static_cast<U*>(m_Ptr), m_RefCount);
        }

        static void Swap(RefPtr& ptrA, RefPtr& ptrB)
        {
            std::swap(ptrA.m_Ptr, ptrB.m_Ptr);
            std::swap(ptrA.m_RefCount, ptrB.m_RefCount);
        }

        template<typename... Args> static RefPtr<T> Create(Args&&... args)
        {
            return RefPtr<T>(new T(std::forward<Args>(args)...));
        }
    private:
        RefPtr(T* ptr, RefCountType* type) : m_Ptr(ptr), m_RefCount(type) {}

        T* m_Ptr;
        RefCountType* m_RefCount;
    };

    template<typename T> class RawRef
    {
    public:
        friend class ScopedPtr<T>;
        friend class RefPtr<T>;
        friend class OwnedPtr<T>;
        static_assert(std::is_pointer_v<T>, "RawRef only supports pointers!");
        template<typename U> friend class RawRef;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        RawRef(std::nullptr_t) : m_Ptr(nullptr) {}

        explicit RawRef(T ptr) : m_Ptr(ptr) {}

        ~RawRef()                        = default;
        RawRef(const RawRef&)            = delete;
        RawRef& operator=(const RawRef&) = delete;

        RawRef(RawRef&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }

        RawRef& operator=(RawRef&& other) noexcept
        {
            if (this != &other)
            {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        explicit operator T*() const { return m_Ptr; }

        T operator->() const noexcept { return m_Ptr; }

        bool operator==(const T* const other) { return m_Ptr == other; }
        template<typename U>
            requires std::is_convertible_v<T, U>
        bool operator==(const std::remove_pointer_t<U>* const other)
        {
            return m_Ptr == static_cast<T>(other);
        }

        // Auto conversion for derived classes
        template<typename U>
            requires std::is_convertible_v<std::remove_pointer_t<T>, std::remove_pointer_t<U>>
        /* NOLINT(google-explicit-constructor, hicpp-explicit-conversions) */ operator RawRef<U>() const
        {
            return RawRef<U>(static_cast<U>(m_Ptr));
        }

        explicit operator T() const noexcept { return m_Ptr; }
    private:
        T m_Ptr;
    };

    template<typename T> class OwnedPtr
    {
    public:
        friend class ScopedPtr<T>;
        friend class RefPtr<T>;
        friend class RawRef<T>;
        using WithoutPtr = std::remove_pointer_t<T>;
        static_assert(std::is_pointer_v<T>, "OwnedPtr only supports pointers!");
        template<typename U> friend class OwnedPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        OwnedPtr(std::nullptr_t) : m_Ptr(nullptr) {}

        explicit OwnedPtr(T ptr) : m_Ptr(ptr) {}

        ~OwnedPtr()                          = default;
        OwnedPtr(const OwnedPtr&)            = delete;
        OwnedPtr& operator=(const OwnedPtr&) = delete;

        OwnedPtr(OwnedPtr&& other) noexcept : m_Ptr(other.m_Ptr) { other.m_Ptr = nullptr; }

        OwnedPtr& operator=(OwnedPtr&& other) noexcept
        {
            if (this != &other)
            {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        explicit operator T*() const { return m_Ptr; }

        T operator->() const noexcept { return m_Ptr; }

        WithoutPtr& operator*() const noexcept { return *m_Ptr; }

        T Get() const noexcept { return m_Ptr; }

        void Reset() { delete m_Ptr; }

        RawRef<T> Raw() const { return RawRef(m_Ptr); }

        bool operator==(const T* const other) { return m_Ptr == other; }
        bool operator==(const RawRef<T>& other) { return m_Ptr == other.m_Ptr; }


        // Auto conversion for derived classes
        template<typename U>
            requires std::is_base_of_v<std::remove_pointer_t<U>, std::remove_pointer_t<T>>
        /* NOLINT(google-explicit-constructor, hicpp-explicit-conversions) */ operator OwnedPtr<U>() const
        {
            return OwnedPtr<U>(static_cast<U>(m_Ptr));
        }

        explicit operator T() const noexcept { return m_Ptr; }

        template<typename... Args> static OwnedPtr<T> Create(Args&&... args)
        {
            return OwnedPtr<T>(new WithoutPtr(std::forward<Args>(args)...));
        }
    private:
        T m_Ptr;
    };
} // namespace Nebula
