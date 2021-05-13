#include "sgpch.h"
#include "GameWorld.h"

#include "ImGuizmo.h"
#include "Sanguine/Events/KeyEvent.h"
#include "Sanguine/Physics/PhysicsEngine.h"  

GameWorld::GameWorld()
    : Layer("3D Sandbox"), m_CameraController(90.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
{

}

GameWorld::~GameWorld()
{

}

void GameWorld::OnAttach()
{
    m_ActiveScene = Sanguine::CreateReference<Sanguine::Scene>();

    Sanguine::FramebufferSpecification frameBufferSpecs;
    frameBufferSpecs.Width = 1280;
    frameBufferSpecs.Height = 720;
    m_Framebuffer = Sanguine::Framebuffer::Create(frameBufferSpecs);

    ConstructDefaultScene();

    Sanguine::SceneRenderer::SetContext(m_ActiveScene);

    Connect("127.0.0.1", 60000);
}

void GameWorld::ConstructDefaultScene()
{
    //Create The Room (Oh hi Mark)
    {
        //Create the floor (GANG AAAAUH!)
        {
            std::string floorID = "floor";
            auto floorEntity = m_ActiveScene->CreateEntity(floorID);

            floorEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.0f);
            floorEntity.GetComponent<Sanguine::TransformComponent>().Update();

            floorEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);

            Sanguine::ColliderState state;
            Sanguine::ColliderType type = Sanguine::ColliderType::Plane;
            Sanguine::ColliderComponent collider(state, type);
            collider.GetState().Position = glm::vec3(0.0f, 0.0f, 0.0f);
            collider.SetNormal(glm::vec3(0.f, 1.f, 0.f));
            collider.SetDistance(0.f);
            floorEntity.AddComponent<Sanguine::ColliderComponent>(collider);
        }
        
        //Create the right wall
        {
            std::string wallID = "wall0";
            auto wallEntity = m_ActiveScene->CreateEntity(wallID);

            wallEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(0.0f, 25.0f, -25.0f); 
            wallEntity.GetComponent<Sanguine::TransformComponent>().Rotation = glm::vec3(90.0f, 0.0f, 0.0f);
            wallEntity.GetComponent<Sanguine::TransformComponent>().Update();

            wallEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);

            Sanguine::ColliderState state;
            Sanguine::ColliderType type = Sanguine::ColliderType::Plane;
            Sanguine::ColliderComponent collider(state, type);
            collider.GetState().Position = glm::vec3(0.0f, 25.0f, -25.0f);
            collider.SetNormal(glm::vec3(0.0f, 0.0f, -1.0f));
            collider.SetDistance(25.0f);
            wallEntity.AddComponent<Sanguine::ColliderComponent>(collider);
        }

        //Create the left wall
        {
            std::string wallID = "wall1";
            auto wallEntity = m_ActiveScene->CreateEntity(wallID);

            wallEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(0.0f, 25.0f, 25.0f);
            wallEntity.GetComponent<Sanguine::TransformComponent>().Rotation = glm::vec3(90.0f, 0.0f, 0.0f);
            wallEntity.GetComponent<Sanguine::TransformComponent>().Update();

            wallEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);

            Sanguine::ColliderState state;
            Sanguine::ColliderType type = Sanguine::ColliderType::Plane;
            Sanguine::ColliderComponent collider(state, type);
            collider.GetState().Position = glm::vec3(0.0f, 25.0f, 25.0f);
            collider.SetNormal(glm::vec3(0.0f, 0.0f, 1.0f));
            collider.SetDistance(25.0f);
            wallEntity.AddComponent<Sanguine::ColliderComponent>(collider);
        }

        //Create the back wall
        {
            std::string wallID = "wall2";
            auto wallEntity = m_ActiveScene->CreateEntity(wallID);

            wallEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(25.0f, 25.0f, 0.0f);
            wallEntity.GetComponent<Sanguine::TransformComponent>().Rotation = glm::vec3(0.0f, 0.0f, 90.0f);
            wallEntity.GetComponent<Sanguine::TransformComponent>().Update();

            wallEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);

            Sanguine::ColliderState state;
            Sanguine::ColliderType type = Sanguine::ColliderType::Plane;
            Sanguine::ColliderComponent collider(state, type);
            collider.GetState().Position = glm::vec3(25.0f, 25.0f, 0.0f);
            collider.SetNormal(glm::vec3(1.0f, 0.0f, 0.0f));
            collider.SetDistance(25.0f);
            wallEntity.AddComponent<Sanguine::ColliderComponent>(collider);
        }

        //Create the ceiling (I spit on the ceiling ლ(｀ー´ლ) )
        {
            std::string ceilingID = "ceiling";
            auto ceilingEntity = m_ActiveScene->CreateEntity(ceilingID);

            ceilingEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(0.0f, 50.0f, 0.0f);
            ceilingEntity.GetComponent<Sanguine::TransformComponent>().Update();

            ceilingEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);
        }
    }

    //Create a default cube
    {
        std::string cubeID = "cube";
        auto cubeEntity = m_ActiveScene->CreateEntity(cubeID);

        cubeEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(5.0f, 1.0f, 5.0f);
        cubeEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
        cubeEntity.GetComponent<Sanguine::TransformComponent>().Update();

        cubeEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Cube);

        Sanguine::ColliderState state;
        Sanguine::ColliderType type = Sanguine::ColliderType::AABB;
        Sanguine::ColliderComponent collider(state, type);
        collider.GetState().Position = glm::vec3(5.0f, 1.0f, 5.0f);
        collider.CalculateExtents();
        cubeEntity.AddComponent<Sanguine::ColliderComponent>(collider);
    }

    //Create a default sphere
    {
        std::string sphereID = "sphere";
        auto sphereEntity = m_ActiveScene->CreateEntity(sphereID);

        sphereEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(3.f, 3.f, 3.f);
        sphereEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(2.0f);
        sphereEntity.GetComponent<Sanguine::TransformComponent>().Update();

        sphereEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Sphere);

        Sanguine::ColliderState state;
        Sanguine::ColliderType type = Sanguine::ColliderType::Sphere;
        Sanguine::ColliderComponent collider(state, type);
        collider.GetState().Position = glm::vec3(3.f, 3.f, 3.f);
        collider.SetCenter(glm::vec3(3.f, 3.f, 3.f));
        collider.SetRadius(1.0f);
        sphereEntity.AddComponent<Sanguine::ColliderComponent>(collider);
    }

    //Create a default teapot
    {
        std::string teapotID = "teapot";
        auto teapotEntity = m_ActiveScene->CreateEntity(teapotID);

        teapotEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(-2.0f, 2.0f, -2.0f);
        teapotEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
        teapotEntity.GetComponent<Sanguine::TransformComponent>().Update();

        teapotEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Teapot);

        Sanguine::ColliderState state;
        Sanguine::ColliderType type = Sanguine::ColliderType::Sphere;
        Sanguine::ColliderComponent collider(state, type);
        collider.GetState().Position = glm::vec3(-2.0f, 2.0f, -2.0f);
        collider.SetCenter(glm::vec3(-2.0f, 2.0f, -2.0f));
        collider.SetRadius(0.25f);
        teapotEntity.AddComponent<Sanguine::ColliderComponent>(collider);

        class TeapotBehaviour : public Sanguine::EntityBehaviour
        {
            virtual void OnCreate() override
            {
                SG_INFO("Teapot Behaviour created!");
            }

            virtual void OnUpdate(Sanguine::Timestep ts) override
            {
                auto& translation = GetComponent<Sanguine::TransformComponent>().Transform.GetTranslation();
            }

            virtual void OnDestroy() override
            {
                SG_INFO("Teapot Behaviour destroyed!");
            }
        };

        teapotEntity.AddComponent<Sanguine::NativeScriptComponent>().Bind<TeapotBehaviour>();
    }
}

void GameWorld::OnDetach()
{

}

void GameWorld::UpdateController(Sanguine::Timestep ts)
{
    float speedFactor = 10.0f;
    glm::vec3 positionOffset{ 0.0f };

    if (Sanguine::Input::IsMouseButtonPressed(SG_MOUSE_BUTTON_2))
    {
        if (Sanguine::Input::IsKeyPressed(SG_KEY_W))
        {
            positionOffset = m_CameraController.GetDirection() * (float)ts;
        }
        else if (Sanguine::Input::IsKeyPressed(SG_KEY_S))
        {
            positionOffset = -1.0f * (m_CameraController.GetDirection() * (float)ts);
        }

        if (Sanguine::Input::IsKeyPressed(SG_KEY_A))
        {
            positionOffset = -1.0f * (glm::normalize(glm::cross(m_CameraController.GetDirection(), glm::vec3{ 0.0f, 1.0f, 0.0f })) * (float)ts);
        }
        else if (Sanguine::Input::IsKeyPressed(SG_KEY_D))
        {
            positionOffset = glm::normalize(glm::cross(m_CameraController.GetDirection(), glm::vec3{ 0.0f, 1.0f, 0.0f })) * (float)ts;
        }

        positionOffset *= speedFactor;
    }

    Sanguine::PhysicsEngine::Get()->AddLinearVelocity(std::string("c_") + std::to_string(m_ClientID), positionOffset, (float)ts);
    glm::vec3 cameraPos = Sanguine::PhysicsEngine::Get()->GetColliderPosition(std::string("c_") + std::to_string(m_ClientID));

    m_CameraController.SetPosition(cameraPos);

    m_CameraController.OnUpdate(ts);

    m_Clients[m_ClientID].Position = m_CameraController.GetPosition();
    m_Clients[m_ClientID].Direction = m_CameraController.GetDirection();
}

void GameWorld::UpdateClient()
{
    if (IsConnected())
    {
        uint32_t msgLimit = 256;
        uint32_t msgCounter = 0;
        while (!Incoming().empty() && msgCounter++ <= msgLimit)
        {
            auto msg = Incoming().pop_front().msg;

            switch (msg.header.id)
            {
            case(GameMsg::Client_Accepted):
            {
                std::cout << "Server accepted client - you're in!\n";
                Sanguine::net::message<GameMsg> msg;
                msg.header.id = GameMsg::Client_RegisterWithServer;

                ClientDescription clientDesc;
                clientDesc.Position = m_CameraController.GetPosition();
                clientDesc.Direction = m_CameraController.GetDirection();

                msg << clientDesc;
                Send(msg);

                break;
            }

            case(GameMsg::Client_AssignID):
            {
                // Server is assigning us OUR id
                msg >> m_ClientID;
                std::cout << "Assigned Client ID = " << m_ClientID << "\n";
                break;
            }

            case(GameMsg::Game_AddPlayer):
            {
                ClientDescription desc;
                msg >> desc;
                m_Clients.insert_or_assign(desc.PeerID, desc);

                if (desc.PeerID == m_ClientID)
                {
                    std::string localClientID = std::string("c_") + std::to_string(desc.PeerID);
                    auto localClientEntity = m_ActiveScene->CreateEntity(localClientID);

                    localClientEntity.GetComponent<Sanguine::TransformComponent>().Translation = desc.Position;
                    localClientEntity.GetComponent<Sanguine::TransformComponent>().Rotation = desc.Direction;
                    localClientEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
                    localClientEntity.GetComponent<Sanguine::TransformComponent>().IsRemoteView = true;

                    Sanguine::ColliderState state;
                    Sanguine::ColliderType type = Sanguine::ColliderType::Sphere;
                    Sanguine::ColliderComponent collider(state, type, true);
                    collider.GetState().Position = desc.Position;
                    collider.SetCenter(desc.Position);
                    collider.SetRadius(0.25f);
                    localClientEntity.AddComponent<Sanguine::ColliderComponent>(collider);

                    // Now we exist in game world
                    m_WaitingOnConnection = false;
                }
                else
                {
                    std::string newClientID = std::string("c_") + std::to_string(desc.PeerID);
                    auto newClientEntity = m_ActiveScene->CreateEntity(newClientID);

                    newClientEntity.GetComponent<Sanguine::TransformComponent>().Translation = desc.Position;
                    newClientEntity.GetComponent<Sanguine::TransformComponent>().Rotation = desc.Direction;
                    newClientEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
                    newClientEntity.GetComponent<Sanguine::TransformComponent>().IsRemoteView = true;

                    newClientEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Teapot);
                    newClientEntity.GetComponent<Sanguine::MeshComponent>().Color = { 0.f, 0.f, 0.f, 1.f };

                    Sanguine::ColliderState state;
                    Sanguine::ColliderType type = Sanguine::ColliderType::Sphere;
                    Sanguine::ColliderComponent collider(state, type, true);
                    collider.GetState().Position = desc.Position;
                    collider.SetCenter(desc.Position);
                    collider.SetRadius(0.25f);
                    newClientEntity.AddComponent<Sanguine::ColliderComponent>(collider);
                }

                break;
            }

            case(GameMsg::Game_RemovePlayer):
            {
                uint32_t nRemovalID = 0;
                msg >> nRemovalID;
                m_Clients.erase(nRemovalID);
                break;
            }

            case(GameMsg::Game_UpdatePlayer):
            {
                ClientDescription desc;
                msg >> desc;
                m_Clients.insert_or_assign(desc.PeerID, desc);

                m_ActiveScene->UpdateClientEntity(desc);
                Sanguine::PhysicsEngine::Get()->SetColliderPosition(std::string("c_") + std::to_string(desc.PeerID), desc.Position);

                break;
            }

            case(GameMsg::Game_ChatMessage):
            {
                ChatMessage message;
                msg >> message;

                m_MessageList.push_back({ message.PeerID, message.Message });

                break;
            }

            }
        }
    }
}

float clockToMilliseconds(clock_t ticks) 
{
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks / (float)CLOCKS_PER_SEC) * 1000.0f;
}

clock_t deltaTime = 0;
unsigned int frames = 0;

void GameWorld::OnUpdate(Sanguine::Timestep ts)
{
    clock_t beginFrame = clock();

    UpdateClient();

    if (m_WaitingOnConnection)
    {
        Sanguine::RendererCommand::SetClearColor(glm::vec4{ 127.5f, 127.5f, 255.f, 1 });
        SG_INFO("Waiting to join the game server...");
        return;
    }

    UpdateController(ts);

    m_Framebuffer->Bind();

    Sanguine::RendererCommand::SetClearColor(glm::vec4{ m_BackgrounColor.x, m_BackgrounColor.y, m_BackgrounColor.z, 1 });
    Sanguine::RendererCommand::Clear();

    m_ActiveScene->OnUpdate(ts, m_CameraController.GetCamera());

    m_Framebuffer->Unbind();

    // Send player description
    Sanguine::net::message<GameMsg> msg;
    msg.header.id = GameMsg::Game_UpdatePlayer;
    msg << m_Clients[m_ClientID];
    Send(msg);

    clock_t endFrame = clock();

    {
        deltaTime += endFrame - beginFrame;
        frames++;

        if (clockToMilliseconds(deltaTime) > 1000.0) 
        { //every second
            m_AverageFrameTimeMilliseconds = 1000.0 / frames;
            frames = 0;
            deltaTime -= CLOCKS_PER_SEC;
        }
    }
}

void GameWorld::OnImGuiRender()
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
	{
        if (ImGui::MenuItem("Exit")) Sanguine::Application::Get().Close();

		ImGui::EndMenuBar();
	}

    //Dockable windows
    RenderSettingsPanel();
    RenderViewSurface();
    RenderChatBox();

	ImGui::End();
}

void GameWorld::RenderSettingsPanel()
{
    ImGui::Begin("Settings");

    ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BackgrounColor));
    ImGui::Text(std::string("FPS:" + std::to_string(m_AverageFrameTimeMilliseconds)).c_str());

    ImGui::End();
}

void GameWorld::RenderViewSurface()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

    ImGui::Begin("Scene View");

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((glm::vec2*)&viewportSize) && viewportSize.x != 0 && viewportSize.y != 0)
    {
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_ViewportSize = { viewportSize.x, viewportSize.y };
        m_CameraController.GetCamera().Resize(m_ViewportSize.x, m_ViewportSize.y);
    }

    uint32_t viewSurface = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)viewSurface, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::PopStyleVar();

    ImGui::End();
}

void GameWorld::RenderChatBox()
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
        m_MessageList.push_back({m_ClientID, chatMessage});

        std::string username = std::string("c_") + std::to_string(m_ClientID);

        ChatMessage message;
        message.PeerID = m_ClientID;
        message.MessageLen = chatMessage.length() + 1;
        message.Message = new char[chatMessage.length() + 1];

        for (uint32_t i = 0; i < message.MessageLen; ++i)
        {
            message.Message[i] = chatMessage[i];
        }
        message.Message[message.MessageLen] = '\0';

        Sanguine::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Game_ChatMessage;
        msg << message;
        Send(msg);
    }

    for (const auto& message : m_MessageList)
    {
        if (message.first == m_ClientID)
        {
            ImGui::Text(std::string("Me: " + message.second).c_str());
        }
        else
        {
            ImGui::Text(std::string("c_" + std::to_string(message.first) + ": " + message.second).c_str());
        }
    }

    ImGui::End();
}

void GameWorld::OnEvent(Sanguine::Event& event)
{
    m_CameraController.OnEvent(event);

    Sanguine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Sanguine::KeyPressedEvent>(SG_BIND_EVENT_FN(OnKeyPressed));
}

bool GameWorld::OnKeyPressed(Sanguine::KeyPressedEvent& e)
{
    return true;
}