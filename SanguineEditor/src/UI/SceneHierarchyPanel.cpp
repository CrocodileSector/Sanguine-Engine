#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

#include "Sanguine/ECS/Components.h"
#include "Sanguine/Physics/Collider.h"
#include "Sanguine/Core/FilesystemUtils.h"
#include "Sanguine/Physics/PhysicsEngine.h"
#include "Sanguine/Renderer/TextureManager.h"

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Sanguine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Reference<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Reference<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();

				if constexpr (std::is_same_v<T, ColliderComponent>)
				{
					PhysicsEngine::Get()->RemoveCollider(entity.GetComponent<TagComponent>().Tag);
				}
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_SelectionContext.HasComponent<CameraComponent>())
					m_SelectionContext.AddComponent<CameraComponent>();
				else
					SG_CORE_WARN("This entity already has the Camera Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Mesh"))
			{
				if (!m_SelectionContext.HasComponent<MeshComponent>())
					m_SelectionContext.AddComponent<MeshComponent>();
				else
					SG_CORE_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Collider"))
			{
				if (!m_SelectionContext.HasComponent<ColliderComponent>())
				{
					Sanguine::ColliderState state;
					Sanguine::ColliderType type = Sanguine::ColliderType::Sphere;
					Sanguine::ColliderComponent collider(state, type);
					collider.GetState().Position = entity.GetComponent<TransformComponent>().Translation;
					collider.SetCenter(entity.GetComponent<TransformComponent>().Translation);
					collider.SetRadius(1.0f);

					m_SelectionContext.AddComponent<ColliderComponent>(collider);
				}
				else
					SG_CORE_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);

				glm::vec3 rotation = component.Rotation;
				DrawVec3Control("Rotation", rotation);

				DrawVec3Control("Scale", component.Scale);

				component.Update();
			});


		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
			{
				if (component.GetMesh())
				{
					const char* meshTypeStrings[] = { "Cube", "Sphere", "Plane", "Teapot", "Custom" };
					int currentMeshType = (int)component.Type;

					if (ImGui::BeginCombo("Mesh Type", meshTypeStrings[(int)currentMeshType]))
					{
						for (int i = 0; i != MeshType::Invalid; ++i)
						{
							bool isSelected = currentMeshType == i;
							if (ImGui::Selectable(meshTypeStrings[i], isSelected))
							{
								if (component.Type != (MeshType)i)
								{
									if ((MeshType)i == MeshType::Custom)
									{
										std::optional<std::string> modelPath = Sanguine::FileDialogs::OpenFile("\0");

										if (modelPath)
										{
											component.Filepath = *modelPath;
										}
										else continue;
									}

									component.Type = (MeshType)i;
									component.ReloadMesh();
								}
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					Sanguine::Reference<Sanguine::Material> material = component.GetMesh()->GetMaterial();

					std::vector<std::string> textureNames;
					std::unordered_map<std::string, Sanguine::Reference<Sanguine::Texture2D>> textures = Sanguine::TextureManager::GetTextures(); 

					for (auto& texture : textures) { textureNames.push_back(texture.first); }
					textureNames.push_back("New...");

					uint32_t textureIdx = material->Texture->GetTextureID();
					if (ImGui::BeginCombo("Texture", textureNames[textureIdx].c_str())) 
					{ 
						for (int i = 0; i < textureNames.size(); ++i) 
						{ 
							bool isSelected = textureIdx == i; 
							if (ImGui::Selectable(textureNames[i].c_str(), isSelected)) 
							{
								if (i == textureNames.size() - 1)
								{
									std::optional<std::string> texturePath = Sanguine::FileDialogs::OpenFile("\0");

									if (texturePath)
									{
										std::filesystem::path filepath(*texturePath);
										material->Texture = Sanguine::TextureManager::LoadTexture(filepath.remove_filename().generic_string(), filepath.filename().generic_string().c_str());
									}
									else continue;
								}
								material->Texture = Sanguine::TextureManager::GetTexture(i);
							} 
							if (isSelected) ImGui::SetItemDefaultFocus(); 
						} 
						ImGui::EndCombo(); 
					}

					textures.clear();
					textureNames.clear();

					float materialAmbient = material->Ambient;
					if (ImGui::DragFloat("Ambient", &materialAmbient))
						material->Ambient = materialAmbient;
					
					float materialSpecular = material->Specular;
					if (ImGui::DragFloat("Specular", &materialSpecular))
						material->Specular = materialSpecular;

					ImGui::ColorEdit3("Color", glm::value_ptr(material->Color));
				}
			});

		DrawComponent<ColliderComponent>("Collider", entity, [&entity](ColliderComponent& component)
			{
				const char* colliderTypeStrings[] = { "Sphere", "AABB", "Plane" };
				int currentColliderType = (int)component.GetType();

				//maybe synch global positions as well :-??
				component.GetState().Position = entity.GetComponent<TransformComponent>().Translation;

				float mass = component.GetState().Mass;
				if (ImGui::DragFloat("Mass", &mass))
					component.GetState().Mass = mass;

				if (ImGui::BeginCombo("Collider Type", colliderTypeStrings[(int)currentColliderType]))
				{
					for (int i = 0; i != (int)ColliderType::Object; ++i)
					{
						bool isSelected = currentColliderType == i;
						if (ImGui::Selectable(colliderTypeStrings[i], isSelected))
						{
							if (component.GetType() != (ColliderType)i)
							{
								if ((ColliderType)i == ColliderType::Sphere)
								{
									component.SetCenter(component.GetState().Position);
									component.SetRadius(1.0f);
								}

								if ((ColliderType)i == ColliderType::AABB)
								{
									component.CalculateExtents();
								}

								if ((ColliderType)i == ColliderType::Plane)
								{
									component.SetNormal(glm::vec3(0.0f));
									component.SetDistance(0.0f);
								}

								component.SetType((ColliderType)i);
							}
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (component.GetType() == ColliderType::Sphere)
				{
					glm::vec3 center = component.GetCenter();
					DrawVec3Control("Center", center);

					float radius = component.GetRadius();
					if (ImGui::DragFloat("Radius", &radius))
						component.SetRadius(radius);
				}

				if (component.GetType() == ColliderType::AABB)
				{
				}

				if (component.GetType() == ColliderType::Plane)
				{
					glm::vec3 normal = component.GetNormal();
					DrawVec3Control("Normal", normal);

					float distance = component.GetDistance();
					if (ImGui::DragFloat("Size", &distance))
						component.SetRadius(distance);
				}

				Sanguine::PhysicsEngine::Get()->UpdateCollider(entity.GetComponent<TagComponent>().Tag, component);
			});
	}

}