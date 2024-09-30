#include <Astrelis/Core/Result.hpp>
#include <benchmark/benchmark.h>

using BMResultType = Astrelis::Result<int, float>;

static void BM_ResultInit(benchmark::State& state) {
    for (auto _state : state) {
        BMResultType result(5);
    }
}

static void BM_ResultUnwrap(benchmark::State& state) {
    BMResultType result(5);
    for (auto _state : state) {
        benchmark::DoNotOptimize(result.Unwrap());
    }
}

static void BM_ResultUnwrapErr(benchmark::State& state) {
    BMResultType result(5.0F);
    for (auto _state : state) {
        benchmark::DoNotOptimize(result.UnwrapErr());
    }
}

static void BM_ResultMapCopy(benchmark::State& state) {
    BMResultType result(5);
    for (auto _state : state) {
        benchmark::DoNotOptimize(result.MapCopy([](int value) { return value + 5; }));
    }
}

static void BM_ResultMapMove(benchmark::State& state) {
    BMResultType result(5);
    for (auto _state : state) {
        benchmark::DoNotOptimize(result.MapMove([](int value) { return value + 5; }));
        // Value is moved from the result
        result = BMResultType(5);
    }
}

BENCHMARK(BM_ResultInit);
BENCHMARK(BM_ResultUnwrap);
BENCHMARK(BM_ResultUnwrapErr);
BENCHMARK(BM_ResultMapCopy);
BENCHMARK(BM_ResultMapMove);
