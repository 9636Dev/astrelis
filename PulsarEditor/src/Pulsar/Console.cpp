#include "Console.hpp"
#include "NebulaEngine/Core/Log.hpp"

#include <imgui.h>

namespace Pulsar
{
    void ConsoleSink::sink_it_(const spdlog::details::log_msg& msg)
    {
        m_Messages.push_back(fmt::format("{}", msg.payload));
        if (m_Messages.size() > m_MaxMessages)
        {
            m_Messages.pop_front();
        }
    }

    void ConsoleSink::flush_() {}

    std::list<std::string>& ConsoleSink::GetMessages()
    {
        return m_Messages;
    }

    Console::Console() : m_Sink(std::make_shared<ConsoleSink>(100))
    {
        Nebula::Log::AddClientSink(m_Sink);
        NEBULA_CORE_LOG_INFO("Console Started!");
    }

    Console::~Console() { Nebula::Log::RemoveClientSink(m_Sink); }

    void Console::Render()
    {
        ImGui::Begin("Console");

        // Scrollable text area
        ImGui::BeginChild("ConsoleScrollingRegion", ImVec2(0, 0), ImGuiChildFlags_None,
                          ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& message : m_Sink->GetMessages())
        {
            ImGui::TextUnformatted(message.c_str());
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace Pulsar
