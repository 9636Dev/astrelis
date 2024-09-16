#pragma once

namespace Astrelis {
    class GlobalConfig {
    public:
        GlobalConfig() = delete;

        static bool IsDebugMode() {
            return s_DebugMode;
        }

        static void SetDebugMode(bool debugMode) {
            s_DebugMode = debugMode;
        }
    private:
        static bool s_DebugMode;
    };
} // namespace Astrelis
