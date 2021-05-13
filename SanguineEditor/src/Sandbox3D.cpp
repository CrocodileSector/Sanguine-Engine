#include "sgpch.h"
#include "Sandbox3D.h"

#include "ImGuizmo.h"
#include "Sanguine/Events/KeyEvent.h"  

Sandbox3D::Sandbox3D()
    : Layer("3D Sandbox"), m_CameraController(90.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
{

}

Sandbox3D::~Sandbox3D()
{

}

void Sandbox3D::OnAttach()
{
    m_ActiveScene = Sanguine::CreateReference<Sanguine::Scene>();

    m_CameraController.SetPosition(m_DefaultCameraPosition);
    m_CameraController.SetDirection(m_DefaultCameraDirection);

    Sanguine::FramebufferSpecification frameBufferSpecs;
    frameBufferSpecs.Width = 1280;
    frameBufferSpecs.Height = 720;
    m_Framebuffer = Sanguine::Framebuffer::Create(frameBufferSpecs);

    ConstructDefaultScene();

    Sanguine::SceneRenderer::SetContext(m_ActiveScene);
    m_SceneHierarchy.SetContext(m_ActiveScene);
}

void Sandbox3D::ConstructDefaultScene()
{
    //Create The Room (Oh hi Mark)
    {
        //Create the floor (GANG AAAAUGH!)
        {
            std::string floorID = "floor";
            auto floorEntity = m_ActiveScene->CreateEntity(floorID);

            floorEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.0f);
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
            ceilingEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Plane);
        }
    }

    //Create a default cube
    {
        std::string cubeID = "cube";
        auto cubeEntity = m_ActiveScene->CreateEntity(cubeID);

        cubeEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(3.0f, 1.0f, 3.0f);
        cubeEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
        cubeEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Cube);
    }

    //Create a default sphere
    {
        std::string sphereID = "sphere";
        auto sphereEntity = m_ActiveScene->CreateEntity(sphereID);

        sphereEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(3.0f, 3.0f, 3.0f);
        sphereEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(2.0f); 
        sphereEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Sphere);
    }

    //Create a default teapot
    {
        std::string teapotID = "teapot";
        auto teapotEntity = m_ActiveScene->CreateEntity(teapotID);

        teapotEntity.GetComponent<Sanguine::TransformComponent>().Translation = glm::vec3(1.0f, 1.0f, 1.0f);
        teapotEntity.GetComponent<Sanguine::TransformComponent>().Scale = glm::vec3(1.0f);
        teapotEntity.AddComponent<Sanguine::MeshComponent>(MeshType::Teapot);

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

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::UpdateController(Sanguine::Timestep ts)
{
    float speedFactor = 2.0f;
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

    m_CameraController.Move(positionOffset);

    m_CameraController.OnUpdate(ts);
}

float clockToMilliseconds(clock_t ticks)
{
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks / (float)CLOCKS_PER_SEC) * 1000.0f;
}

clock_t deltaTime = 0;
unsigned int frames = 0;

void Sandbox3D::OnUpdate(Sanguine::Timestep ts)
{
    clock_t beginFrame = clock();

    UpdateController(ts);

    m_Framebuffer->Bind();

    Sanguine::RendererCommand::SetClearColor(glm::vec4{ m_BackgrounColor.x, m_BackgrounColor.y, m_BackgrounColor.z, 1 });
    Sanguine::RendererCommand::Clear();

    m_ActiveScene->OnUpdate(ts, m_CameraController.GetCamera());

    m_Framebuffer->Unbind();

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

void Sandbox3D::OnImGuiRender()
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
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Sanguine::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

    m_SceneHierarchy.OnImGuiRender();

    //Dockable windows
    RenderSettingsPanel();
    RenderViewSurface();

	ImGui::End();
}

void Sandbox3D::RenderSettingsPanel()
{
    ImGui::Begin("Settings");

    ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BackgrounColor));

    glm::vec3 localRight = glm::normalize(glm::cross(m_CameraController.GetDirection(), { 0.0f, 1.0f, 0.0f }));

    ImGui::InputFloat3("Camera Position", glm::value_ptr(m_CameraController.GetPosition()), "%.3f", 0);
    ImGui::InputFloat3("Camera Direction", glm::value_ptr(m_CameraController.GetDirection()), "%.3f", 0);
    ImGui::InputFloat3("Camera Right", glm::value_ptr(localRight), "%.3f", 0);

    ImGui::Text(std::string("FPS:" + std::to_string(m_AverageFrameTimeMilliseconds)).c_str());

    ImGui::End();
}

void Sandbox3D::RenderViewSurface()
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

    // Gizmos
    Sanguine::Entity selectedEntity = m_SceneHierarchy.GetSelectedEntity();
    if (selectedEntity && m_GizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        
        const glm::mat4& cameraProjection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 cameraView = m_CameraController.GetCamera().GetViewMatrix();

        // Entity transform
        auto& tc = selectedEntity.GetComponent<Sanguine::TransformComponent>();
        glm::mat4 transform = tc.Transform.GetModel();

        // Snapping
        bool snap = Sanguine::Input::IsKeyPressed(SG_KEY_LEFT_CONTROL);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale

        // Snap to 45 degrees for rotation
        if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Sanguine::Math::DecomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += glm::radians(deltaRotation);
            tc.Scale = scale;

            tc.Update();
        }
    }

    ImGui::PopStyleVar();

    ImGui::End();
}

void Sandbox3D::OnEvent(Sanguine::Event& event)
{
	m_CameraController.OnEvent(event);

    Sanguine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Sanguine::KeyPressedEvent>(SG_BIND_EVENT_FN(OnKeyPressed));
}

bool Sandbox3D::OnKeyPressed(Sanguine::KeyPressedEvent& e)
{
    // Shortcuts
    if (e.GetRepeatCount() > 0)
        return false;

    bool control = Sanguine::Input::IsKeyPressed(SG_KEY_LEFT_CONTROL) || Sanguine::Input::IsKeyPressed(SG_KEY_RIGHT_CONTROL);
    bool shift = Sanguine::Input::IsKeyPressed(SG_KEY_LEFT_SHIFT) || Sanguine::Input::IsKeyPressed(SG_KEY_RIGHT_SHIFT);
    switch (e.GetKeyCode())
    {
    case SG_KEY_N:
    {
        if (control)
            NewScene();

        break;
    }
    case SG_KEY_O:
    {
        if (control)
            OpenScene();

        break;
    }
    case SG_KEY_S:
    {
        if (control && shift)
            SaveSceneAs();

        break;
    }

    // Gizmos
    case SG_KEY_Q:
        m_GizmoType = -1;
        break;
    case SG_KEY_W:
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        break;
    case SG_KEY_E:
        m_GizmoType = ImGuizmo::OPERATION::ROTATE;
        break;
    case SG_KEY_R:
        m_GizmoType = ImGuizmo::OPERATION::SCALE;
        break;
    }
    
    return true;
}

void Sandbox3D::NewScene()
{
    m_ActiveScene = Sanguine::CreateReference<Sanguine::Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    Sanguine::SceneRenderer::SetContext(m_ActiveScene);
    m_SceneHierarchy.SetContext(m_ActiveScene);
}

void Sandbox3D::OpenScene()
{
    std::optional<std::string> filepath = Sanguine::FileDialogs::OpenFile("Sanguine Scene (*.sng)\0*.sng\0");
    if (filepath)
    {
        m_ActiveScene = Sanguine::CreateReference<Sanguine::Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        Sanguine::SceneRenderer::SetContext(m_ActiveScene);
        m_SceneHierarchy.SetContext(m_ActiveScene);

        Sanguine::SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize(*filepath);
    }
}

void Sandbox3D::SaveSceneAs()
{
    std::optional<std::string> filepath = Sanguine::FileDialogs::SaveFile("Sanguine Scene (*.sng)\0*.sng\0");
    if (filepath)
    {
        Sanguine::SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(*filepath);
    }
}