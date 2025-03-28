#include "Console.hpp"

#include "Astrelis/Core/Log.hpp"

#include <imgui.h>

namespace AstrelisEditor {
    void ConsoleSink::sink_it_(const spdlog::details::log_msg& msg) {
        m_Messages.push_back(fmt::format("{}", msg.payload));
        if (m_Messages.size() > m_MaxMessages) {
            m_Messages.pop_front();
        }
    }

    void ConsoleSink::flush_() {
    }

    std::list<std::string>& ConsoleSink::GetMessages() {
        return m_Messages;
    }

    Console::Console() : m_Sink(std::make_shared<ConsoleSink>(100)) {
        Astrelis::Log::AddClientSink(m_Sink);
        ASTRELIS_CORE_LOG_INFO("Console Started!");
    }

    Console::~Console() {
        Astrelis::Log::RemoveClientSink(m_Sink);
    }

    void Console::Render() {
        ImGui::Begin("Console");

        // Scrollable text area
        ImGui::BeginChild("ConsoleScrollingRegion", ImVec2(0, 0), ImGuiChildFlags_None,
            ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& message : m_Sink->GetMessages()) {
            ImGui::TextUnformatted(message.c_str());
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace AstrelisEditor
