#include <benchmark/benchmark.h>

#include <array>

#include "NebulaCore/Util/Pointer.hpp"

using Nebula::Ptr;

struct TestStruct
{
    std::array<int, 1'024> data;
};

static void BM_PointerCreate(benchmark::State& state)
{
    for (auto _unused : state)
    {
        Ptr<TestStruct> ptr(new TestStruct());
        benchmark::DoNotOptimize(ptr);
    }
}

BENCHMARK(BM_PointerCreate)->MinTime(10.0); // NOLINT

static void BM_RawPtrCreate(benchmark::State& state)
{
    for (auto _unused : state)
    {
        gsl::owner<TestStruct*> ptr = new TestStruct();
        benchmark::DoNotOptimize(ptr);
        delete ptr;
    }
}

BENCHMARK(BM_RawPtrCreate)->MinTime(10.0); // NOLINT

static void BM_PtrCreateAndDeref(benchmark::State& state)
{
    for (auto _unused : state)
    {
        Ptr<int> ptr(new int(5));
        benchmark::DoNotOptimize(*ptr);
    }
}

BENCHMARK(BM_PtrCreateAndDeref)->MinTime(10.0); // NOLINT

static void BM_RawPtrCreateAndDeref(benchmark::State& state)
{
    for (auto _unused : state)
    {
        gsl::owner<int*> ptr = new int(5);
        benchmark::DoNotOptimize(*ptr);
        delete ptr;
    }
}

BENCHMARK(BM_RawPtrCreateAndDeref)->MinTime(10.0); // NOLINT

static void BM_PointerCreateRef(benchmark::State& state)
{
    Ptr<int> ptr(new int(5));
    benchmark::DoNotOptimize(ptr);
    for (auto _unused : state)
    {
        auto ref = ptr.GetRef();
        benchmark::DoNotOptimize(ref);
    }
}

BENCHMARK(BM_PointerCreateRef)->MinTime(10.0); // NOLINT

static void BM_RawPtrCreateRef(benchmark::State& state)
{
    gsl::owner<int*> ptr = new int(5);
    for (auto _unused : state)
    {
        auto* ref = ptr;
        benchmark::DoNotOptimize(ref);
    }
    delete ptr;
}

BENCHMARK(BM_RawPtrCreateRef)->MinTime(10.0); // NOLINT
