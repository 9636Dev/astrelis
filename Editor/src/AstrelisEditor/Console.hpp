#pragma once

#include <list>
#include <memory>
#include <spdlog/sinks/base_sink.h>

namespace AstrelisEditor {
    class ConsoleSink : public spdlog::sinks::base_sink<std::mutex> {
    public:
        explicit ConsoleSink(std::size_t maxMessages = 100) : m_MaxMessages(maxMessages) {
        }

        std::list<std::string>& GetMessages();
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override;
        void flush_() override;
    private:
        std::size_t            m_MaxMessages;
        std::list<std::string> m_Messages;
    };

    class Console {
    public:
        Console();
        ~Console();
        Console(const Console&)            = delete;
        Console& operator=(const Console&) = delete;
        Console(Console&&) noexcept        = delete;
        Console& operator=(Console&&)      = delete;

        void Render();
    private:
        std::shared_ptr<ConsoleSink> m_Sink;
    };
} // namespace AstrelisEditor
