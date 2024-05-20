#pragma once

#include "NebulaCore/Util/TypeTraits.hpp"
#include <functional>
#include <variant>

namespace Nebula
{
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

        using IsSameType               = std::bool_constant<std::is_same_v<T, E>>;
        using TCopyConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowCopyConstructible && !IsSameType::value>;
        using ECopyConstructorNoexcept = std::bool_constant<ETypeProps::NoThrowCopyConstructible && !IsSameType::value>;
        using TMoveConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowMoveConstructible &&
                                                            !IsSameType::value && !TTypeProps::TriviallyCopyable>;
        using EMoveConstructorNoexcept = std::bool_constant<ETypeProps::NoThrowMoveConstructible &&
                                                            !IsSameType::value && !ETypeProps::TriviallyCopyable>;
        using TCopyConstructor =
            std::bool_constant<!TCopyConstructorNoexcept::value && TTypeProps::CopyConstructible && !IsSameType::value>;
        using ECopyConstructor =
            std::bool_constant<!ECopyConstructorNoexcept::value && ETypeProps::CopyConstructible && !IsSameType::value>;
        using TMoveConstructor = std::bool_constant<!TMoveConstructorNoexcept::value && TTypeProps::MoveConstructible &&
                                                    !IsSameType::value && !TTypeProps::TriviallyCopyable>;
        using EMoveConstructor = std::bool_constant<!EMoveConstructorNoexcept::value && ETypeProps::MoveConstructible &&
                                                    !IsSameType::value && !ETypeProps::TriviallyCopyable>;
        using SCopyConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowCopyConstructible && IsSameType::value>;
        using SCopyConstructor =
            std::bool_constant<!SCopyConstructorNoexcept::value && TTypeProps::CopyConstructible && IsSameType::value>;
        using SMoveConstructorNoexcept = std::bool_constant<TTypeProps::NoThrowMoveConstructible && IsSameType::value>;
        using SMoveConstructor =
            std::bool_constant<!SMoveConstructorNoexcept::value && TTypeProps::MoveConstructible && IsSameType::value>;
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

        [[nodiscard]] bool IsOk() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return m_Value.isOk;
            }
            return std::holds_alternative<T>(m_Value);
        }

        [[nodiscard]] bool IsErr() const noexcept
        {
            if constexpr (IsSameType::value)
            {
                return !m_Value.isOk;
            }
            return std::holds_alternative<E>(m_Value);
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
            return std::get<T>(m_Value);
        }

        /**
        * @see Unwrap()
        */
        [[nodiscard]] const T& Unwrap() const noexcept { return std::get<T>(m_Value); }

        /**
        * @brief Unwraps the error value of the result
        * @note Without checks, this function can cause undefined behavior
        */
        [[nodiscard]] E& UnwrapErr() noexcept { return std::get<E>(m_Value); }

        /**
        * @see UnwrapErr()
        */
        [[nodiscard]] const E& UnwrapErr() const noexcept { return std::get<E>(m_Value); }

        template<typename F>
            requires(TTypeProps::MoveConstructible && ETypeProps::MoveConstructible)
        auto MapMove(F func) -> Result<decltype(func(std::declval<T>())), E>
        {
            if (IsOk())
            {
                if constexpr (std::is_same_v<decltype(func(std::declval<T>())), T>)
                {
                    return Result<decltype(func(std::move(Unwrap()))), E>(func(std::move(Unwrap())), true);
                }
                return Result<decltype(func(std::declval<T>())), E>(func(std::move(Unwrap())));
            }
            if constexpr (IsSameType::value)
            {
                return Result<decltype(func(std::declval<T>())), E>(std::move(UnwrapErr()), false);
            }
            return std::move(UnwrapErr());
        }

        template<typename F>
            requires(TTypeProps::CopyConstructible && ETypeProps::CopyConstructible)
        auto MapCopy(F func) -> Result<decltype(func(std::declval<T>())), E>
        {
            if (IsOk())
            {
                if constexpr (std::is_same_v<decltype(func(std::declval<T>())), T>)
                {
                    return Result<decltype(func(Unwrap())), E>(func(Unwrap()), true);
                }
                return Result<decltype(func(Unwrap())), E>(func(Unwrap()));
            }
            if constexpr (IsSameType::value)
            {
                return Result<decltype(func(std::declval<T>())), E>(UnwrapErr(), false);
            }
            return UnwrapErr();
        }
    private:
        Type m_Value;

        static_assert(!TTypeProps::Abstract, "T cannot be an abstract class");
        static_assert(!ETypeProps::Abstract, "E cannot be an abstract class");
    };
}; // namespace Nebula
