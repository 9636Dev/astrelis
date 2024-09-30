#include <Astrelis/Core/Pointer.hpp>
#include <benchmark/benchmark.h>

#include <memory>

static void BM_UniqPtrCreateDelete(benchmark::State& state) {
    for (auto _state : state) {
        auto ptr = std::make_unique<int>(5);
        ptr.reset();
        benchmark::DoNotOptimize(ptr);
    }
}

static void BM_UniqPtrDeref(benchmark::State& state) {
    auto ptr = std::make_unique<int>(5);
    for (auto _state : state) {
        auto deref = *ptr;
        benchmark::DoNotOptimize(deref);
    }
}

static void BM_ScopedPtrCreateDelete(benchmark::State& state) {
    for (auto _state : state) {
        auto ptr = Astrelis::ScopedPtr<int>::Create(5);
        ptr.Reset();
        benchmark::DoNotOptimize(ptr);
    }
}

static void BM_ScopedPtrDeref(benchmark::State& state) {
    auto ptr = Astrelis::ScopedPtr<int>::Create(5);
    for (auto _state : state) {
        auto deref = *ptr;
        benchmark::DoNotOptimize(deref);
    }
}


BENCHMARK(BM_UniqPtrCreateDelete);
BENCHMARK(BM_UniqPtrDeref);

BENCHMARK(BM_ScopedPtrCreateDelete);
BENCHMARK(BM_ScopedPtrDeref);

static void BM_SharedPtrCreateDelete(benchmark::State& state) {
    for (auto _state : state) {
        auto ptr = std::make_shared<int>(5);
        ptr.reset();
        benchmark::DoNotOptimize(ptr);
    }
}

static void BM_SharedPtrCopy(benchmark::State& state) {
    auto ptr = std::make_shared<int>(5);
    for (auto _state : state) {
        auto copy = ptr;
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_SharedPtrDeref(benchmark::State& state) {
    auto ptr = std::make_shared<int>(5);
    for (auto _state : state) {
        auto deref = *ptr;
        benchmark::DoNotOptimize(deref);
    }
}

static void BM_RefPtrCreateDelete(benchmark::State& state) {
    for (auto _state : state) {
        auto ptr = Astrelis::RefPtr<int>::Create(5);
        ptr.Reset();
        benchmark::DoNotOptimize(ptr);
    }
}

static void BM_RefPtrCopy(benchmark::State& state) {
    auto ptr = Astrelis::RefPtr<int>::Create(5);
    for (auto _state : state) {
        auto copy = ptr;
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_RefPtrDeref(benchmark::State& state) {
    auto ptr = Astrelis::RefPtr<int>::Create(5);
    for (auto _state : state) {
        auto deref = *ptr;
        benchmark::DoNotOptimize(deref);
    }
}

BENCHMARK(BM_SharedPtrCreateDelete);
BENCHMARK(BM_SharedPtrCopy);
BENCHMARK(BM_SharedPtrDeref);

BENCHMARK(BM_RefPtrCreateDelete);
BENCHMARK(BM_RefPtrCopy);
BENCHMARK(BM_RefPtrDeref);

