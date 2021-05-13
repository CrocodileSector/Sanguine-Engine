#include "sgpch.h"

#include "GameWorld.h"

#include <Sanguine/Core/EntryPoint.h>

Sanguine::RendererAPI::API Sanguine::RendererAPI::s_API = Sanguine::RendererAPI::API::OpenGL;

class Sandbox : public Sanguine::Application
{
public:
	Sandbox() 
	{
		PushLayer(new GameWorld());
	}

    /*
    void RenderChatBox()
    {
        ImGui::Begin("Chatroom");

        ImGui::Text("Chat:");
        ImGui::SameLine();

        std::string chatMessage = "";
        char messageBuf[256];
        memset(messageBuf, 0, sizeof(messageBuf));
        std::strncpy(messageBuf, chatMessage.c_str(), sizeof(messageBuf));
        if (ImGui::InputText("##Chat:", messageBuf, sizeof(messageBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            chatMessage = std::string(messageBuf);
            m_ChatMessageList.push_back(chatMessage);
        }

        for (const auto& message : m_ChatMessageList)
        {
            ImGui::Text(std::string("Me: " + message).c_str());
        }

        ImGui::End();
    }
    */
    
	~Sandbox()
	{

	}

private:
	std::list<std::string> m_ChatMessageList;
};

Sanguine::Application* Sanguine::CreateApplication()
{
	return new Sandbox();
}