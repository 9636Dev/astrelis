#pragma once

#include "NebulaCore/Util/TypeTraits.hpp"
#include <variant>
#include <forward_list>

namespace Nebula
{
    /**
     * @brief A result type that can hold either a value or an error
     * @tparam T The value type
     * @tparam E The error type
     * @note The value and error types must be copy or move constructible
     */
    template<typename T, typename E> class Result
    {
    private:
        struct SameType
        {
            T value;
            bool isOk;
        };

        using TTypeProps = TypeProperties<T>;
        using ETypeProps = TypeProperties<E>;

        using OneIsNonCopyable = std::bool_constant<!TTypeProps::CopyConstructible || !ETypeProps::CopyConstructible>;
        using OneIsNonMoveable = std::bool_constant<!TTypeProps::MoveConstructible || !ETypeProps::MoveConstructible>;
        using IsSameType       = std::bool_constant<std::is_same_v<T, E>>;
        using TCopyConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowCopyConstructible && !IsSameType::value>;
        using ECopyConstructorNoexcept = std::bool_constant<ETypeProps::NoThrowCopyConstructible && !IsSameType::value>;
        using TMoveConstructorNoexcept =
            std::bool_constant<TTypeProps::NoThrowMoveConstructible && !IsSameType::value &&
                               !(TTypeProps::TriviallyCopyable && TTypeProps::CopyConstructible)>;
        using EMoveConstructorNoexcept =
            std::bool_constant<ETypeProps::NoThrowMoveConstructible && !IsSameType::value &&
                               !(ETypeProps::TriviallyCopyable && ETypeProps::CopyConstructible)>;
        using TCopyConstructor =
            std::bool_constant<!TCopyConstructorNoexcept::value && TTypeProps::CopyConstructible && !IsSameType::value>;
        using ECopyConstructor =
            std::bool_constant<!ECopyConstructorNoexcept::value && ETypeProps::CopyConstructible && !IsSameType::value>;
        using TMoveConstructor =
            std::bool_constant<!TMoveConstructorNoexcept::value && TTypeProps::MoveConstructible &&
                               !IsSameType::value && !(TTypeProps::TriviallyCopyable && TTypeProps::CopyConstructible)>;
        using EMoveConstructor =
            std::bool_constant<!EMoveConstructorNoexcept::value && ETypeProps::MoveConstructible &&
                               !IsSameType::value && !(ETypeProps::TriviallyCopyable && ETypeProps::CopyConstructible)>;
        using SCopyConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowCopyConstructible && IsSameType::value>;
        using SCopyConstructor =
            std::bool_constant<!SCopyConstructorNoexcept::value && TTypeProps::CopyConstructible && IsSameType::value>;
        using SMoveConstructorNoexcept =
            std::bool_constant<TTypeProps::NoThrowMoveConstructible && IsSameType::value &&
                               !(TTypeProps::TriviallyCopyable && TTypeProps::CopyConstructible)>;
        using SMoveConstructor =
            std::bool_constant<!SMoveConstructorNoexcept::value && TTypeProps::MoveConstructible && IsSameType::value &&
                               !(TTypeProps::TriviallyCopyable && TTypeProps::CopyConstructible)>;
    public:
        using VariantType = std::variant<T, E>;
        using Type        = std::conditional_t<IsSameType::value, SameType, VariantType>;
        using ResultType  = Result<T, E>;

        // === Constructors ===
        Result(T value) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires TCopyConstructorNoexcept::value
            : m_Value(value)
        {
        }

        Result(E value) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires ECopyConstructorNoexcept::value
            : m_Value(value)
        {
        }

        Result(T&& value) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires TMoveConstructorNoexcept::value
            : m_Value(std::move(value))
        {
        }

        Result(E&& value) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires EMoveConstructorNoexcept::value
            : m_Value(std::move(value))
        {
        }

        Result(T value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires TCopyConstructor::value
            : m_Value(value)
        {
        }

        Result(E value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires ECopyConstructor::value
            : m_Value(value)
        {
        }

        Result(T&& value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires TMoveConstructor::value
            : m_Value(std::move(value))
        {
        }

        Result(E&& value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires EMoveConstructor::value
            : m_Value(std::move(value))
        {
        }

        Result(T value, bool isOk = true) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires SCopyConstructorNoexcept::value
            : m_Value(SameType {value, isOk})
        {
        }

        Result(T value, bool isOk = true) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires SCopyConstructor::value
            : m_Value(SameType {value, isOk})
        {
        }

        Result(T&& value, bool isOk = true) noexcept // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires SMoveConstructorNoexcept::value
            : m_Value(SameType {std::move(value), isOk})
        {
        }

        Result(T&& value, bool isOk = true) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires SMoveConstructor::value
            : m_Value(SameType {std::move(value), isOk})
        {
        }

        // === Destructor ===

        /**
         * @note Because the destructor is trivial, so not handling a result type with a pointer might cause a memory leak
         */
        ~Result() = default;

        // === Other Constructors ===

        Result(const ResultType& other)
            requires(TTypeProps::CopyConstructible && ETypeProps::CopyConstructible)
            : m_Value(other.m_Value)
        {
        }

        Result(const ResultType& other) noexcept
            requires(TTypeProps::NoThrowCopyConstructible && ETypeProps::NoThrowCopyConstructible)
            : m_Value(other.m_Value)
        {
        }

        Result(ResultType&& other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(TTypeProps::MoveConstructible && ETypeProps::MoveConstructible)
            : m_Value(std::move(other.m_Value))
        {
        }

        Result(ResultType&& other) noexcept
            requires(TTypeProps::NoThrowMoveConstructible && ETypeProps::NoThrowMoveConstructible)
            : m_Value(std::move(other.m_Value))
        {
        }

        // === Operators ===

        ResultType&
            operator=(const ResultType& other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(TTypeProps::CopyConstructible && ETypeProps::CopyConstructible)
        {
            if (this == &other)
            {
                return *this;
            }
            m_Value = other.m_Value;
            return *this;
        }

        ResultType& operator=(ResultType&& other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(TTypeProps::MoveConstructible && ETypeProps::MoveConstructible)
        {
            if (this == &other)
            {
                return *this;
            }
            m_Value = std::move(other.m_Value);
            return *this;
        }

        ResultType& operator=(const ResultType& other) noexcept
            requires(TTypeProps::NoThrowCopyConstructible && ETypeProps::NoThrowCopyConstructible)
        {
            if (this == &other)
            {
                return *this;
            }
            m_Value = other.m_Value;
            return *this;
        }

        ResultType& operator=(ResultType&& other) noexcept
            requires(TTypeProps::NoThrowMoveConstructible && ETypeProps::NoThrowMoveConstructible)
        {
            if (this == &other)
            {
                return *this;
            }
            m_Value = std::move(other.m_Value);
            return *this;
        }

        // === Methods ===

        /**
         * @brief Checks if the result is Ok
         * @return True if the result is Ok, false otherwise
         */
        [[nodiscard]] bool IsOk() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.isOk;
            }
            else
            {
                return std::holds_alternative<T>(m_Value);
            }
        }

        /**
         * @brief Checks if the result is an error
         * @return True if the result is an error, false otherwise
         */
        [[nodiscard]] bool IsErr() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return !m_Value.isOk;
            }
            else
            {
                return std::holds_alternative<E>(m_Value);
            }
        }

        /**
        * @brief Unwraps the value of the result
        * @note Without checks, this function can cause undefined behavior
        */
        [[nodiscard]] T& Unwrap() noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.value;
            }
            else
            {
                return std::get<T>(m_Value);
            }
        }

        /**
        * @see Unwrap()
        */
        [[nodiscard]] const T& Unwrap() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.value;
            }
            else
            {
                return std::get<T>(m_Value);
            }
        }

        /**
        * @brief Unwraps the error value of the result
        * @note Without checks, this function can cause undefined behavior
        */
        [[nodiscard]] E& UnwrapErr() noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.value;
            }
            else
            {
                return std::get<E>(m_Value);
            }
        }

        /**
        * @see UnwrapErr()
        */
        [[nodiscard]] const E& UnwrapErr() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.value;
            }
            else
            {
                return std::get<E>(m_Value);
            }
        }

        /**
        * @brief Maps the value of the result
        * @tparam F The function type
        * @param func The function to map the value
        * @return The result of the function
        * @note This function is only available if the type is move constructible
        */
        template<typename F>
            requires(TTypeProps::MoveConstructible && ETypeProps::MoveConstructible)
        auto MapMove(F func) -> Result<decltype(func(std::declval<T>())), E>
        {
            using type = decltype(func(std::move(std::declval<T>())));
            if (IsOk())
            {
                if constexpr (std::is_same_v<type, E>)
                {
                    return Result<type, E>(func(std::move(Unwrap())), true);
                }
                else
                {
                    return Result<type, E>(func(std::move(Unwrap())));
                }
            }
            if constexpr (IsSameType::value)
            {
                return Result<type, E>(std::move(UnwrapErr()), false);
            }
            else
            {
                return std::move(UnwrapErr());
            }
        }

        /**
        * @brief Maps the value of the result
        * @tparam F The function type
        * @param func The function to map the value
        * @return The result of the function
        * @note This function is only available if the type is copy constructible
        */
        template<typename F>
            requires(TTypeProps::CopyConstructible && ETypeProps::CopyConstructible)
        auto MapCopy(F func) -> Result<decltype(func(static_cast<const T&>(std::declval<T>()))), E>
        {
            // This is tricky to implement, because we need to prevent return optimization
            using type = decltype(func(static_cast<const T&>(std::declval<T>())));
            if (IsOk())
            {
                if constexpr (std::is_same_v<type, E>)
                {
                    return Result<type, E>(func(Unwrap()), true);
                }
                else
                {
                    return Result<type, E>(func(Unwrap()));
                }
            }
            if constexpr (IsSameType::value)
            {
                return Result<type, E>(UnwrapErr(), false);
            }
            else
            {
                return UnwrapErr();
            }
        }
    private:
        Type m_Value;

        static_assert(!TTypeProps::Abstract, "T cannot be an abstract class");
        static_assert(!ETypeProps::Abstract, "E cannot be an abstract class");
        static_assert(TTypeProps::CopyConstructible || TTypeProps::MoveConstructible, "T must be copy or move constructible");
        static_assert(ETypeProps::CopyConstructible || ETypeProps::MoveConstructible, "E must be copy or move constructible");
    };

    template<typename T, typename E, typename ...Args>
    Result<T, E> MakeOkResult(Args&& ...args)
    {
        if constexpr (std::is_same_v<T, E>)
        {
            return Result<T, E>(T(std::forward<Args>(args)...), true);
        }
        else
        {
            return Result<T, E>(T(std::forward<Args>(args)...));
        }
    }

    template<typename T, typename E, typename ...Args>
    Result<T, E> MakeErrResult(Args&& ...args)
    {
        if constexpr (std::is_same_v<T, E>)
        {
            return Result<T, E>(T(std::forward<Args>(args)...), false);
        }
        else
        {
            return Result<T, E>(E(std::forward<Args>(args)...));
        }
    }
}; // namespace Nebula
