#pragma once

//To be used by client applications

//////////////////////////////
//////		Core	   //////
////////////////////////////
#include "Sanguine/Core/Core.h"
#include "Sanguine/Core/Input.h"
#include "Sanguine/Core/Layer.h"
#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Core/KeyCodes.h"
#include "Sanguine/Core/MathUtils.h"
#include "Sanguine/Core/Application.h"
#include "Sanguine/Core/FilesystemUtils.h"
#include "Sanguine/Core/MouseButtonCodes.h"

#include "Sanguine/Events/Event.h"
#include "Sanguine/Events/KeyEvent.h"
#include "Sanguine/Events/AppEvent.h"
#include "Sanguine/Events/MouseEvent.h"

//////////////////////////////
//////	   Renderer	   //////
////////////////////////////
#include "Sanguine/Renderer/Camera.h"
#include "Sanguine/Renderer/Buffer.h"
#include "Sanguine/Renderer/Shader.h"
#include "Sanguine/Renderer/Texture.h"
#include "Sanguine/Renderer/Renderer.h"
#include "Sanguine/Renderer/Renderer2D.h"
#include "Sanguine/Renderer/Framebuffer.h"
#include "Sanguine/Renderer/RendererAPI.h"
#include "Sanguine/Renderer/VertexArray.h"
#include "Sanguine/Renderer/TextureManager.h"
#include "Sanguine/Renderer/RendererCommand.h"
#include "Sanguine/Renderer/PerspectiveCamera.h"
#include "Sanguine/Renderer/OrthographicCamera.h"
#include "Sanguine/Renderer/PerspectiveCameraController.h"

//////////////////////////////
//////	     ECS	   //////
////////////////////////////
#include "Sanguine/ECS/Scene.h"
#include "Sanguine/ECS/Entity.h"
#include "Sanguine/ECS/Components.h"
#include "Sanguine/ECS/EntityBehaviour.h"
#include "Sanguine/ECS/SceneSerializer.h"

//////////////////////////////
//////	    Network	   //////
////////////////////////////
#include "Sanguine/Network/NetCommon.h"

//////////////////////////////
////// Others/3rdparty //////
////////////////////////////
#include "Sanguine/ImGui/ImGuiLayer.h"

#include "Sanguine/OpenGL/OpenGLMesh.h"
#include "Sanguine/OpenGL/OpenGLShader.h"

#include "Sanguine/Physics/Collider.h"
#include "Sanguine/Physics/PhysicsEngine.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>