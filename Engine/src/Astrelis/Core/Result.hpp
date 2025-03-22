#pragma once

#include <stdexcept>
#include <type_traits>
#include <variant>

// Code Snippet from:
// https://gist.github.com/HW9636/04aed78dd9eeb754259ad7a0755ed273

namespace Astrelis {

    template<typename T, typename E> class Result {
    private:
        struct SameType {
            T    value;
            bool isOk;
        };

        using IsSameType = std::bool_constant<std::is_same_v<T, E>>;
    public:
        using VariantType = std::variant<T, E>;
        using Type        = std::conditional_t<IsSameType::value, SameType, VariantType>;
        using ResultType  = Result<T, E>;

        // === Constructors ===

        template<typename U>
        Result(U&& value,
            bool success = true) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires(std::is_same_v<T, E>)
            : m_Value {std::forward<U>(value), success} {
        }

        template<typename U>
        Result(U&& value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires(!std::is_same_v<T, E>)
            : m_Value {std::forward<U>(value)} {
        }

        // === Destructor ===

        /**
         * @note Because the destructor is trivial, so not handling a result type with a pointer might cause a memory leak
         */
        ~Result() = default;

        // === Other Constructors ===

        Result(const ResultType& other)
            requires(std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E>)
            : m_Value(other.m_Value) {
        }

        Result(const ResultType& other) noexcept
            requires(
                std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
            : m_Value(other.m_Value) {
        }

        Result(
            ResultType&& other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(std::is_move_constructible_v<T> && std::is_move_constructible_v<E>
                && !std::is_nothrow_move_constructible_v<T>
                && !std::is_nothrow_move_constructible_v<E>)
            : m_Value(std::move(other.m_Value)) {
        }

        Result(ResultType&& other) noexcept
            requires(
                std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
            : m_Value(std::move(other.m_Value)) {
        }

        // === Operators ===

        ResultType& operator=(const ResultType&
                other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E>)
        {
            if (this == &other) {
                return *this;
            }
            m_Value = other.m_Value;
            return *this;
        }

        ResultType& operator=(
            ResultType&& other) // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
            requires(std::is_move_constructible_v<T> && std::is_move_constructible_v<E>
                && !std::is_nothrow_move_constructible_v<T>
                && !std::is_nothrow_move_constructible_v<E>)
        {
            if (this == &other) {
                return *this;
            }
            m_Value = std::move(other.m_Value);
            return *this;
        }

        ResultType& operator=(const ResultType& other) noexcept
            requires(
                std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
        {
            if (this == &other) {
                return *this;
            }
            m_Value = other.m_Value;
            return *this;
        }

        ResultType& operator=(ResultType&& other) noexcept
            requires(
                std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
        {
            if (this == &other) {
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
        [[nodiscard]] bool IsOk() const noexcept {
            if constexpr (IsSameType::value) {
                return m_Value.isOk;
            }
            else {
                return std::holds_alternative<T>(m_Value);
            }
        }

        /**
         * @brief Checks if the result is an error
         * @return True if the result is an error, false otherwise
         */
        [[nodiscard]] bool IsErr() const noexcept {
            if constexpr (IsSameType::value) {
                return !m_Value.isOk;
            }
            else {
                return std::holds_alternative<E>(m_Value);
            }
        }

        /**
        * @brief Unwraps the value of the result
        * @note Without checks, this function can cause undefined behavior
        */
        [[nodiscard]] T& Unwrap() noexcept {
            if constexpr (IsSameType::value) {
                return m_Value.value;
            }
            else {
                return std::get<T>(m_Value);
            }
        }

        /**
        * @see Unwrap()
        */
        [[nodiscard]] const T& Unwrap() const noexcept {
            if constexpr (IsSameType::value) {
                return m_Value.value;
            }
            else {
                return std::get<T>(m_Value);
            }
        }

        /**
        * @brief Unwraps the error value of the result
        * @note Without checks, this function can cause undefined behavior
        */
        [[nodiscard]] E& UnwrapErr() noexcept {
            if constexpr (IsSameType::value) {
                return m_Value.value;
            }
            else {
                return std::get<E>(m_Value);
            }
        }

        /**
        * @see UnwrapErr()
        */
        [[nodiscard]] const E& UnwrapErr() const noexcept {
            if constexpr (IsSameType::value) {
                return m_Value.value;
            }
            else {
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
            requires(std::is_move_constructible_v<T> && std::is_move_constructible_v<E>)
        auto MapMove(F func) -> Result<decltype(func(std::declval<T>())), E> {
            using type = decltype(func(std::move(std::declval<T>())));
            if (IsOk()) {
                if constexpr (std::is_same_v<type, E>) {
                    return Result<type, E>(func(std::move(Unwrap())), true);
                }
                else {
                    return Result<type, E>(func(std::move(Unwrap())));
                }
            }
            if constexpr (IsSameType::value) {
                return Result<type, E>(std::move(UnwrapErr()), false);
            }
            else {
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
            requires(std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E>)
        auto MapCopy(F func)
            -> Result<decltype(func(static_cast<const T&>(std::declval<T>()))), E> {
            // This is tricky to implement, because we need to prevent return optimization
            using type = decltype(func(static_cast<const T&>(std::declval<T>())));
            if (IsOk()) {
                if constexpr (std::is_same_v<type, E>) {
                    return Result<type, E>(func(Unwrap()), true);
                }
                else {
                    return Result<type, E>(func(Unwrap()));
                }
            }
            if constexpr (IsSameType::value) {
                return Result<type, E>(UnwrapErr(), false);
            }
            else {
                return UnwrapErr();
            }
        }

        template<typename... Args> static Result<T, E> Ok(Args&&... args) {
            if constexpr (std::is_same_v<T, E>) {
                return ResultType(T(std::forward<Args>(args)...), true);
            }
            else {
                return ResultType(T(std::forward<Args>(args)...));
            }
        }

        template<typename... Args> static Result<T, E> Err(Args&&... args) {
            if constexpr (std::is_same_v<T, E>) {
                return ResultType(T(std::forward<Args>(args)...), false);
            }
            else {
                return ResultType(E(std::forward<Args>(args)...));
            }
        }

        T& Expect(const std::string& message) {
            if (IsOk()) {
                return Unwrap();
            }
            throw std::runtime_error(message);
        }

        const T& Expect(const std::string& message) const {
            if (IsOk()) {
                return Unwrap();
            }
            throw std::runtime_error(message);
        }

        E& ExpectErr(const std::string& message) {
            if (IsErr()) {
                return UnwrapErr();
            }
            throw std::runtime_error(message);
        }

        const E& ExpectErr(const std::string& message) const {
            if (IsErr()) {
                return UnwrapErr();
            }
            throw std::runtime_error(message);
        }
    private:
        Type m_Value;

        static_assert(!std::is_abstract_v<T>, "T cannot be an abstract class");
        static_assert(!std::is_abstract_v<E>, "E cannot be an abstract class");
        static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>,
            "T must be copy or move constructible");
        static_assert(std::is_copy_constructible_v<E> || std::is_move_constructible_v<E>,
            "E must be copy or move constructible");
    };

    struct EmptyType {};

} // namespace Astrelis
