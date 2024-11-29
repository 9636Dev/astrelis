#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace Astrelis {
    // Forward declarations
    template<typename T> class ScopedPtr;
    template<typename T> class RefPtr;
    template<typename T> class OwnedPtr;
    template<typename T> class RawRef;

    /**
    * @brief A smart pointer implementation, similar to 'std::unique_ptr'
    * The destruction of the ScopedPtr is handled by the smart pointer destructor, so the user does not need to delete it manually.
    */
    template<typename T> class ScopedPtr {
    public:
        static_assert(!std::is_array_v<T>, "ScopedPtr does not support arrays!");
        static_assert(!std::is_reference_v<T>, "ScopedPtr does not support references!");
        template<typename U> friend class ScopedPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        ScopedPtr(std::nullptr_t = nullptr) : m_Ptr(nullptr) {
        }

        explicit ScopedPtr(T* ptr) : m_Ptr(ptr) {
        }

        ~ScopedPtr() {
            delete m_Ptr;
        }

        ScopedPtr(const ScopedPtr&)            = delete;
        ScopedPtr& operator=(const ScopedPtr&) = delete;

        ScopedPtr(ScopedPtr&& other) noexcept : m_Ptr(other.m_Ptr) {
            other.m_Ptr = nullptr;
        }

        ScopedPtr& operator=(ScopedPtr&& other) noexcept {
            if (this != &other) {
                delete m_Ptr;
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        T* operator->() const noexcept {
            return m_Ptr;
        }

        T& operator*() const noexcept {
            return *m_Ptr;
        }

        T* Get() const noexcept {
            return m_Ptr;
        }

        bool operator==(const ScopedPtr& other) const noexcept {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const ScopedPtr& other) const noexcept {
            return m_Ptr != other.m_Ptr;
        }

        void Reset() {
            delete m_Ptr;
            m_Ptr = nullptr;
        }

        template<typename U>
            requires std::is_base_of_v<U, T>
        explicit operator ScopedPtr<U>() {
            T* ptr = m_Ptr;
            m_Ptr  = nullptr;
            return ScopedPtr<U>(static_cast<U*>(ptr));
        }

        template<typename... Args> static ScopedPtr<T> Create(Args&&... args) {
            return ScopedPtr<T>(new T(std::forward<Args>(args)...));
        }
    private:
        T* m_Ptr;
    };

    using RefCountType = std::size_t;

    /**
    * @brief A reference counted smart pointer
    */
    template<typename T> class RefPtr {
    public:
        static_assert(!std::is_array_v<T>, "RefPtr does not support arrays!");
        static_assert(!std::is_reference_v<T>, "RefPtr does not support references!");
        template<typename U> friend class RefPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        RefPtr(std::nullptr_t = nullptr) : m_Ptr(nullptr), m_RefCount(nullptr) {
        }

        explicit RefPtr(T* ptr) : m_Ptr(ptr), m_RefCount(new RefCountType(1)) {
        }

        ~RefPtr() {
            if (m_RefCount != nullptr && --(*m_RefCount) == 0) {
                delete m_Ptr;
                delete m_RefCount;
            }
        }

        RefPtr(const RefPtr& other) : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount) {
            if (m_RefCount != nullptr) {
                ++(*m_RefCount);
            }
        }

        RawRef<T*> Raw() const {
            return RawRef<T*>(m_Ptr);
        }

        RefPtr& operator=(const RefPtr& other) {
            if (this != &other) {
                if (m_RefCount != nullptr && --(*m_RefCount) == 0) {
                    delete m_Ptr;
                    delete m_RefCount;
                }
                m_Ptr      = other.m_Ptr;
                m_RefCount = other.m_RefCount;
                if (m_RefCount != nullptr) {
                    ++(*m_RefCount);
                }
            }
            return *this;
        }

        RefPtr(RefPtr&& other) noexcept : m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount) {
            other.m_Ptr      = nullptr;
            other.m_RefCount = nullptr;
        }

        RefPtr& operator=(RefPtr&& other) noexcept {
            if (this != &other) {
                if (m_RefCount != nullptr && --(*m_RefCount) == 0) {
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

        T* operator->() const noexcept {
            return m_Ptr;
        }

        T& operator*() const noexcept {
            return *m_Ptr;
        }

        T* Get() const noexcept {
            return m_Ptr;
        }

        bool operator==(const RefPtr& other) const noexcept {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const RefPtr& other) const noexcept {
            return m_Ptr != other.m_Ptr;
        }

        void Reset() {
            if (m_RefCount != nullptr && --(*m_RefCount) == 0) {
                delete m_Ptr;
                delete m_RefCount;
            }
            m_Ptr      = nullptr;
            m_RefCount = nullptr;
        }

        void ResetNoDelete() noexcept {
            m_Ptr      = nullptr;
            m_RefCount = nullptr;
        }

        template<typename U>
            requires std::is_base_of_v<U, T>
        explicit operator RefPtr<U>() const {
            if (m_RefCount != nullptr) {
                ++(*m_RefCount);
            }
            return RefPtr<U>(static_cast<U*>(m_Ptr), m_RefCount);
        }

        template<typename U> RefPtr<U> DynamicCast() {
            if (m_RefCount != nullptr) {
                ++(*m_RefCount);
            }
            return RefPtr<U>(dynamic_cast<U*>(m_Ptr), m_RefCount);
        }

        template<typename U> RefPtr<U> As() const {
            if (m_RefCount != nullptr) {
                ++(*m_RefCount);
            }
            return RefPtr<U>(reinterpret_cast<U*>(m_Ptr), m_RefCount);
        }

        static void Swap(RefPtr& ptrA, RefPtr& ptrB) {
            std::swap(ptrA.m_Ptr, ptrB.m_Ptr);
            std::swap(ptrA.m_RefCount, ptrB.m_RefCount);
        }

        template<typename... Args> static RefPtr<T> Create(Args&&... args) {
            return RefPtr<T>(new T(std::forward<Args>(args)...));
        }
    private:
        RefPtr(T* ptr, RefCountType* type) : m_Ptr(ptr), m_RefCount(type) {
        }

        T*            m_Ptr;
        RefCountType* m_RefCount;
    };

    template<typename T> class RawRef {
    public:
        friend class ScopedPtr<T>;
        friend class RefPtr<T>;
        friend class OwnedPtr<T>;
        static_assert(std::is_pointer_v<T>, "RawRef only supports pointers!");
        template<typename U> friend class RawRef;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        RawRef([[maybe_unused]] std::nullptr_t ptr = nullptr) : m_Ptr(nullptr) {
        }

        explicit RawRef(T ptr) : m_Ptr(ptr) {
        }

        explicit RawRef(std::remove_pointer_t<T>& obj) : m_Ptr(&obj) {
        }

        ~RawRef() = default;

        RawRef(const RawRef& other) : m_Ptr(other.m_Ptr) {
        }

        template<typename U>
            requires std::is_pointer_v<U>
        RawRef<U> As() const {
            return RawRef<U>(reinterpret_cast<U>(m_Ptr));
        }

        RawRef& operator=(const RawRef& other) {
            if (this != &other) {
                m_Ptr = other.m_Ptr;
            }
            return *this;
        }

        RawRef(RawRef&& other) noexcept : m_Ptr(other.m_Ptr) {
            other.m_Ptr = nullptr;
        }

        RawRef& operator=(RawRef&& other) noexcept {
            if (this != &other) {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        operator T() const {
            return m_Ptr;
        }

        T operator->() const noexcept {
            return m_Ptr;
        }

        bool operator==(const std::remove_pointer_t<T>* const other) const noexcept {
            return m_Ptr == other;
        }

        bool operator!=(const std::remove_pointer_t<T>* const other) const noexcept {
            return m_Ptr != other;
        }

        bool operator==(const RawRef other) const noexcept {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const RawRef other) const noexcept {
            return m_Ptr != other.m_Ptr;
        }

        template<typename U>
            requires std::is_convertible_v<T, U>
        bool operator==(const std::remove_pointer_t<U>* const other) const noexcept {
            return m_Ptr == static_cast<T>(other);
        }

        template<typename U>
            requires std::is_convertible_v<T, U>
        bool operator!=(const std::remove_pointer_t<U>* const other) const noexcept {
            return m_Ptr != static_cast<T>(other);
        }

        // Auto conversion for derived classes
        template<typename U>
            requires std::is_convertible_v<std::remove_pointer_t<T>, std::remove_pointer_t<U>>
        explicit operator RawRef<U>() const {
            return RawRef<U>(static_cast<U>(m_Ptr));
        }
    private:
        T m_Ptr;
    };

    /**
    * @brief A non-smart pointer, but indicates the ownership of the pointer, responsible for deleting it.
    */
    template<typename T> class OwnedPtr {
    public:
        friend class ScopedPtr<T>;
        friend class RefPtr<T>;
        friend class RawRef<T>;
        using WithoutPtr = std::remove_pointer_t<T>;
        static_assert(std::is_pointer_v<T>, "OwnedPtr only supports pointers!");
        template<typename U> friend class OwnedPtr;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        OwnedPtr(std::nullptr_t) : m_Ptr(nullptr) {
        }

        explicit OwnedPtr(T ptr) : m_Ptr(ptr) {
        }

        ~OwnedPtr()                          = default;
        OwnedPtr(const OwnedPtr&)            = delete;
        OwnedPtr& operator=(const OwnedPtr&) = delete;

        OwnedPtr(OwnedPtr&& other) noexcept : m_Ptr(other.m_Ptr) {
            other.m_Ptr = nullptr;
        }

        OwnedPtr& operator=(OwnedPtr&& other) noexcept {
            if (this != &other) {
                m_Ptr       = other.m_Ptr;
                other.m_Ptr = nullptr;
            }
            return *this;
        }

        explicit operator T*() const {
            return m_Ptr;
        }

        T operator->() const noexcept {
            return m_Ptr;
        }

        WithoutPtr& operator*() const noexcept {
            return *m_Ptr;
        }

        T Get() const noexcept {
            return m_Ptr;
        }

        void Reset() {
            delete m_Ptr;
            m_Ptr = nullptr;
        }

        RawRef<T> Raw() const {
            return RawRef(m_Ptr);
        }

        bool operator==(const std::remove_pointer_t<T>* const other) const noexcept {
            return m_Ptr == other;
        }

        bool operator!=(const std::remove_pointer_t<T>* const other) const noexcept {
            return m_Ptr != other;
        }

        bool operator==(const RawRef<T>& other) const noexcept {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const RawRef<T>& other) const noexcept {
            return m_Ptr != other.m_Ptr;
        }

        bool operator==(const OwnedPtr& other) const noexcept {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const OwnedPtr& other) const noexcept {
            return m_Ptr != other.m_Ptr;
        }

        // Auto conversion for derived classes
        template<typename U>
            requires std::is_base_of_v<std::remove_pointer_t<U>, std::remove_pointer_t<T>>
        explicit operator OwnedPtr<U>() {
            T ptr = m_Ptr;
            m_Ptr = nullptr;
            return OwnedPtr<U>(static_cast<U>(ptr));
        }

        explicit operator T() const noexcept {
            return m_Ptr;
        }

        template<typename... Args> static OwnedPtr<T> Create(Args&&... args) {
            return OwnedPtr<T>(new WithoutPtr(std::forward<Args>(args)...));
        }
    private:
        T m_Ptr;
    };

    /**
    * @brief A raw, unsafe pointer
    * @note This type is never used in the renderer, but the user can use it in their applications
    */
    template<typename T>
        requires std::is_pointer_v<T>
    using UnsafeRef = T;
} // namespace Astrelis
