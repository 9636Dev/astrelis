#pragma once

#include <stdexcept>

// TODO(9636D): Doesn't compile

namespace Nebula
{
    template<typename T, typename R> class ResultSuccess;
    template<typename T, typename R> class ResultError;

    // Inspired by the Result class from the Rust programming language
    template<typename T, typename R> class Result
    {
    public:
        static ResultSuccess<T, R> Ok(T value) { return ResultSuccess<T, R>(value); }

        static ResultError<T, R> Err(T value) { return ResultError<T, R>(value); }

        [[nodiscard]] virtual bool IsSuccess() const = 0;
        [[nodiscard]] virtual bool IsError() const   = 0;

        [[nodiscard]] virtual T Unwrap() const      = 0;
        [[nodiscard]] virtual R UnwrapError() const = 0;
    };

    template<typename T, typename R> class ResultSuccess : public Result<T, R>
    {
    public:
        explicit ResultSuccess(T value) : m_Value(value) {}

        [[nodiscard]] bool IsSuccess() const override { return true; }

        [[nodiscard]] bool IsError() const override { return false; }

        [[nodiscard]] T Unwrap() const override { return m_Value; }

        [[nodiscard]] R UnwrapError() const override { throw std::logic_error("Cannot unwrap error from success"); }

        T m_Value;
    };

    template<typename T, typename R> class ResultError : public Result<T, R>
    {
    public:
        explicit ResultError(R value) : m_Value(value) {}

        [[nodiscard]] bool IsSuccess() const override { return false; }

        [[nodiscard]] bool IsError() const override { return true; }

        [[nodiscard]] T Unwrap() const override { throw std::logic_error("Cannot unwrap value from error"); }

        [[nodiscard]] R UnwrapError() const override { return m_Value; }

        R m_Value;
    };
} // namespace Nebula
