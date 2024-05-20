#pragma once

#include <functional>
#include <variant>

namespace Nebula
{
    /**
    * @brief A Result type that can hold either a value or an error
    * @tparam T The type of the value
    * @tparam E The type of the error
    */
    template<typename T, typename E> class Result
    {
    private:
        explicit constexpr Result(const T& value, bool isOk)
            requires(std::is_same_v<T, E>)
            : m_Value({isOk, value})
        {
        }
    public:
        constexpr Result(const T& value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires(!std::is_same_v<T, E>)
            : m_Value(value)
        {
        }

        constexpr Result(const E& value) // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
            requires(!std::is_same_v<T, E>)
            : m_Value(value)
        {
        }

        /**
        * @brief Creates a new Result with a value
        * @param value The value to store
        * @return A new Result with the value
        */
        constexpr static Result<T, E> Ok(const T& value)
        {
            if constexpr (std::is_same_v<T, E>)
            {
                return Result<T, E>(value, true);
            }
            return Result<T, E>(value);
        }

        /**
        * @brief Creates a new Result with an error
        * @param value The error to store
        * @return A new Result with the error
        */
        constexpr static Result<T, E> Err(const E& value)
        {
            if constexpr (std::is_same_v<T, E>)
            {
                return Result<T, E>(value, false);
            }
            return Result<T, E>(value);
        }

        /**
        * @brief Checks if the Result is an ok value
        * @return True if the Result is an ok value, false otherwise
        */
        constexpr bool IsOk() const
        {
            if constexpr (std::is_same_v<T, E>)
            {
                return m_Value.m_IsOk;
            }
            return std::holds_alternative<T>(m_Value);
        }

        /**
        * @brief Checks if the Result is an error
        * @return True if the Result is an error, false otherwise
        */
        constexpr bool IsErr() const
        {
            if constexpr (std::is_same_v<T, E>)
            {
                return !m_Value.m_IsOk;
            }
            return std::holds_alternative<E>(m_Value);
        }

        /**
        * @brief Unwraps the value from the Result
        * @return The value stored in the Result
        * @throws std::runtime_error if the Result is an error
        * @throws std::bad_variant_access if the Result is not an ok value
        */
        constexpr T Unwrap()
        {
            if constexpr (std::is_same_v<T, E>)
            {
                if (!m_Value.m_IsOk)
                {
                    throw std::runtime_error("Cannot unwrap an error");
                }
                return m_Value.m_Value;
            }
            return std::get<T>(m_Value);
        }

        /**
         * @brief Unwraps the error from the Result
         * @return The error stored in the Result
         * @throws std::runtime_error if the Result is an ok value
        * @throws std::bad_variant_access if the Result is not an error
        */
        constexpr E UnwrapErr()
        {
            if constexpr (std::is_same_v<T, E>)
            {
                if (m_Value.m_IsOk)
                {
                    throw std::runtime_error("Cannot unwrap an ok value");
                }
                return m_Value.m_Value;
            }
            return std::get<E>(m_Value);
        }

        /**
        * @brief Maps the value of the Result to a new value
        * @param func The function to apply to the value
        * @return A new Result with the mapped value
        * @note May throw exceptions, @see Unwrap
        */
        template<typename R> Result<R, E> Map(std::function<R(T)> func)
        {
            if (IsErr())
            {
                return Result<R, E>::Err(UnwrapErr());
            }

            R result = func(Unwrap());
            return Result<R, E>::Ok(result);
        }
    private:
        struct SameType
        {
            bool m_IsOk;
            T m_Value;
        };

        using Type = std::conditional_t<std::is_same_v<T, E>, SameType, std::variant<T, E>>;
        Type m_Value;
    };

}; // namespace Nebula
