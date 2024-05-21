#include <benchmark/benchmark.h>
#include <string>

#include "NebulaCore/Util/Result.hpp"

using Nebula::Result;

static void BM_ResultCreateOk(benchmark::State& state)
{
    for (auto _unused : state)
    {
        Result<int, std::string> result(5);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ResultCreateOk); // NOLINT

static void BM_ResultCreateErr(benchmark::State& state)
{
    std::string data = "Error";
    for (auto _unused : state)
    {
        Result<int, std::string> result(data);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ResultCreateErr); // NOLINT

static void BM_ResultCreateErrMove(benchmark::State& state)
{
    std::string data = "Error";
    for (auto _unused : state)
    {
        Result<int, std::string> result(std::move(data));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ResultCreateErrMove); // NOLINT

static void BM_ResultMapCopy(benchmark::State& state)
{
    Result<int, std::string> result(5);
    for (auto _unused : state)
    {
        benchmark::DoNotOptimize(result.MapCopy([](int var) { return var + 1; }));
    }
}

BENCHMARK(BM_ResultMapCopy); // NOLINT
