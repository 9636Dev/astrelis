#include <benchmark/benchmark.h>

#include "NebulaCore/Log/Log.hpp"

#include <spdlog/sinks/null_sink.h>

static void InitLoggers()
{
    static bool s_Initialized = false;
    if (s_Initialized)
    {
        return;
    }

    Nebula::Log::Init();
    auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
    auto& logger = Nebula::Log::GetClientLogger();
    logger->sinks().clear();
    logger->sinks().push_back(null_sink);
    s_Initialized = true;
}

static void BM_Log(benchmark::State& state)
{
    InitLoggers();

    for (auto _unused : state)
    {
        NEB_LOG_INFO("This is a log message");
    }
}

BENCHMARK(BM_Log); // NOLINT
