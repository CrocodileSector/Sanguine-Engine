#pragma once

#include <vector>
#include <glfw/glfw3.h>

#include "Collider.h"

#include "Sanguine/Core/Core.h"
#include "Sanguine/Core/Timestep.h"
#include "Sanguine/Network/NetCommon.h"


namespace Sanguine
{
	class PhysicsEngine
	{
	public:

		void RemoveCollider(const std::string& id)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");
			m_Colliders.erase(id);
		}

		void UpdateCollider(const std::string& id, ColliderComponent& comp)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");
			m_Colliders[id] = comp;
		}

		void AddLinearVelocity(std::string id, const glm::vec3& offset, float delta = 0.f)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");

			ColliderState state = m_Colliders[id].GetState();
			state.LinearVelocity += offset;

			m_Colliders[id].SetState(state);

			HandleCollisions(delta, id);
			m_Colliders[id].Update(delta);
		}

		void OffsetCollider(std::string id, const glm::vec3& offset)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");

			ColliderState state = m_Colliders[id].GetState();
			state.Position += offset;

			m_Colliders[id].SetState(state);
		}

		glm::vec3& GetColliderPosition(std::string id)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");

			return m_Colliders[id].GetState().Position;
		}

		static Reference<PhysicsEngine> Get()
		{
			if (s_PhysicsEngine == nullptr) s_PhysicsEngine = CreateReference<PhysicsEngine>();

			return s_PhysicsEngine;
		}

		void Register(const std::string& id, const ColliderComponent& collider)
		{
			m_Colliders[id] = collider;
		}

		ColliderComponent GetCollider(const std::string& id)
		{	
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");

			return m_Colliders[id];
		}

		void SetColliderPosition(const std::string& id, const glm::vec3& pos)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");

			m_Colliders[id].GetState().LastPosition = m_Colliders[id].GetState().Position;
			m_Colliders[id].GetState().Position = pos;
			m_Colliders[id].Transform(m_Colliders[id].GetState().Position - m_Colliders[id].GetState().LastPosition);
		}

		~PhysicsEngine()
		{

		}

		void Update(float delta)
		{
			if (m_Colliders.size() == 0xDDDDDDDDDDDDDDDD) return;

			for (auto& object : m_Colliders)
			{
				object.second.Update(delta);
			}

			HandleCollisions(delta);
		}


		CollisionData SphereToSphere(ColliderComponent& sphere, ColliderComponent& otherSphere)
		{
			float radiusDistance = (sphere.GetRadius() + otherSphere.GetRadius()) * (sphere.GetRadius() + otherSphere.GetRadius());

			glm::vec3 direction = (otherSphere.GetCenter() - sphere.GetCenter());
			float centerDistance = glm::dot(direction, direction);
			centerDistance *= centerDistance;

			direction /= centerDistance;

			float distance = centerDistance - radiusDistance;

			return CollisionData(centerDistance <= radiusDistance, direction * distance, CollisionType::SphereToSphere);
		}

		CollisionData SphereToAABB(ColliderComponent& sphere, ColliderComponent& AABB)
		{
			glm::vec3 aabbMin = AABB.GetMinExtents();
			glm::vec3 aabbMax = AABB.GetMaxExtents();
			glm::vec3 closestDst{ 0.0f };
			for (int i = 0; i < 3; i++)
			{
				float v = sphere.GetCenter()[i];
				if (v < aabbMin[i]) v = aabbMin[i];
				if (v > aabbMax[i]) v = aabbMax[i];
				closestDst[i] = v;
			}

			glm::vec3 direction = (closestDst - sphere.GetCenter());
			float centerDistance = glm::dot(direction, direction);
			centerDistance *= centerDistance;

			return CollisionData(centerDistance < sphere.GetRadius(), closestDst, CollisionType::SphereToAABB);
		}

		CollisionData AABBToAABB(ColliderComponent& AABB, ColliderComponent& otherAABB)
		{
			bool collides = false;
			glm::vec3 distance{ 0.0f };

			if ((AABB.GetMaxExtents().x > otherAABB.GetMinExtents().x && AABB.GetMinExtents().x < otherAABB.GetMaxExtents().x) &&
				(AABB.GetMaxExtents().y > otherAABB.GetMinExtents().y && AABB.GetMinExtents().y < otherAABB.GetMaxExtents().y) &&
				(AABB.GetMaxExtents().z > otherAABB.GetMinExtents().z && AABB.GetMinExtents().z < otherAABB.GetMaxExtents().z))
			{
				collides = true;

				glm::vec3 distances1 = otherAABB.GetMinExtents() - AABB.GetMaxExtents();
				glm::vec3 distances2 = AABB.GetMinExtents() - otherAABB.GetMaxExtents();

				distance = glm::normalize(distances1 - distances2);
			}

			return CollisionData(collides, distance, CollisionType::AABBToAABB);
		}

		CollisionData PlaneToSphere(ColliderComponent& plane, ColliderComponent& sphere)
		{
			float distanceFromSphereCenter = glm::abs(glm::dot(sphere.GetCenter(), plane.GetNormal()) - plane.GetDistance());

			return CollisionData(distanceFromSphereCenter < 1.f, sphere.GetCenter() - plane.GetNormal(), CollisionType::SphereToPlane);
		}

		CollisionData PlaneToAABB(ColliderComponent& plane, ColliderComponent& AABB)
		{
			glm::vec3 center = (AABB.GetMaxExtents() + AABB.GetMinExtents()) * 0.5f;
			glm::vec3 extents = (AABB.GetMaxExtents() - center);

			float radius = glm::abs(extents.x * plane.GetNormal().x) + glm::abs(extents.y * plane.GetNormal().y) + glm::abs(extents.z * plane.GetNormal().z);
			float distanceFromSphereCenter = glm::dot(center, plane.GetNormal()) - plane.GetDistance();

			bool collides = true;
			if (distanceFromSphereCenter > 1.0f || distanceFromSphereCenter < -1.0f)
			{
				collides = false;
			}

			return CollisionData(collides, AABB.GetCenter() - plane.GetNormal(), CollisionType::AABBToPlane);
		}

		void HandleCollisions(float delta, std::string id)
		{
			if (m_Colliders.find(id) == m_Colliders.end()) SG_CORE_ASSERT(true, "Detected an invalid collider map access...");
			auto firstCollider = m_Colliders[id];

			for (auto& secondCollider : m_Colliders)
			{
				if (id == secondCollider.first) continue;

				CollisionData collisionData;

				if (firstCollider.GetType() == ColliderType::Plane)
				{
					switch (secondCollider.second.GetType())
					{
					case ColliderType::AABB:
					{
						collisionData = PlaneToAABB(firstCollider, secondCollider.second);
						break;
					}

					case ColliderType::Sphere:
					{
						collisionData = PlaneToSphere(firstCollider, secondCollider.second);
						break;
					}
					}
				}
				else if (firstCollider.GetType() == ColliderType::Sphere)
				{
					switch (secondCollider.second.GetType())
					{
					case ColliderType::AABB:
					{
						collisionData = SphereToAABB(firstCollider, secondCollider.second);
						break;
					}

					case ColliderType::Sphere:
					{
						collisionData = SphereToSphere(firstCollider, secondCollider.second);
						break;
					}
					}
				}
				else if (firstCollider.GetType() == ColliderType::AABB)
				{
					switch (secondCollider.second.GetType())
					{
					case ColliderType::AABB:
					{
						collisionData = AABBToAABB(firstCollider, secondCollider.second);
						break;
					}

					case ColliderType::Sphere:
					{
						collisionData = SphereToAABB(secondCollider.second, firstCollider);
						break;
					}
					}
				}

				if (collisionData.DoesCollide())
				{
					ResolveCollision(delta, collisionData, firstCollider, secondCollider.second);
				}
			}
		}

		void HandleCollisions(float delta)
		{
			if (m_Colliders.size() <= 1) return;

			for (auto& firstCollider : m_Colliders)
			{
				for (auto& secondCollider : m_Colliders)
				{
					CollisionData collisionData;

					if (firstCollider.second.GetType() == ColliderType::Plane)
					{
						switch (secondCollider.second.GetType())
						{
						case ColliderType::AABB:
						{
							collisionData = PlaneToAABB(firstCollider.second, secondCollider.second);
							break;
						}

						case ColliderType::Sphere:
						{
							collisionData = PlaneToSphere(firstCollider.second, secondCollider.second);
							break;
						}
						}
					}
					else if (firstCollider.second.GetType() == ColliderType::Sphere)
					{
						switch (secondCollider.second.GetType())
						{
						case ColliderType::AABB:
						{
							collisionData = SphereToAABB(firstCollider.second, secondCollider.second);
							break;
						}

						case ColliderType::Sphere:
						{
							collisionData = SphereToSphere(firstCollider.second, secondCollider.second);
							break;
						}
						}
					}
					else if (firstCollider.second.GetType() == ColliderType::AABB)
					{
						switch (secondCollider.second.GetType())
						{
						case ColliderType::AABB:
						{
							collisionData = AABBToAABB(firstCollider.second, secondCollider.second);
							break;
						}

						case ColliderType::Sphere:
						{
							collisionData = SphereToAABB(secondCollider.second, firstCollider.second);
							break;
						}
						}
					}

					if (collisionData.DoesCollide())
					{
						ResolveCollision(delta, collisionData, firstCollider.second, secondCollider.second);
					}

					if (!isRunning) return;
				}
			}
		}

		void ResolveCollision(float timestep, const CollisionData& collision, ColliderComponent& collider, ColliderComponent& otherCollider)
		{
			ColliderState stateA = collider.GetState();
			ColliderState stateB = otherCollider.GetState();

			glm::vec3 posA = stateA.Position;
			glm::vec3 posB = stateB.Position;

			glm::vec3 deltaA = stateA.LinearVelocity + glm::cross(stateA.AngularVelocity, posA);
			glm::vec3 deltaB = stateB.LinearVelocity + glm::cross(stateB.AngularVelocity, posB);

			glm::vec3 delta = deltaB - deltaA;
			glm::vec3 collisionNormal = glm::normalize(collision.GetDirection());

			glm::vec3 collisionDirA = glm::cross(stateA.InverseInertia * glm::cross(posA, collisionNormal), posA);
			glm::vec3 collisionDirB = glm::cross(stateB.InverseInertia * glm::cross(posB, collisionNormal), posB);

			glm::vec3 dv = collisionDirB - collisionDirA;

			float collisionAngle = glm::dot(collisionNormal, collisionDirA + collisionDirB);
			float massConstraint = stateA.InverseMass + stateB.InverseMass + collisionAngle;

			float baumgarteScalar = 0.1f;
			float baumgarteSlope = 0.001f;
			float penetrationSlope = collision.GetDistanceMagnitude() + baumgarteSlope;

			float bCoeff = -(baumgarteScalar / timestep) * penetrationSlope;

			float elasticity = 0.1f;
			float elasticityRatio = glm::dot(collisionNormal, stateA.LinearVelocity + glm::cross(posA, stateA.AngularVelocity) - stateB.LinearVelocity - glm::cross(posB, stateB.AngularVelocity));

			bCoeff += elasticity * elasticityRatio;

			float impulse = glm::max(-glm::dot(dv, collisionNormal) + glm::abs(bCoeff), 0.0f);

			impulse = glm::clamp(impulse, 0.0f, 1.0f);

			switch (collision.GetType())
			{
				case CollisionType::ObjectToAABB:
				case CollisionType::ObjectToSphere:
				case CollisionType::SphereToSphere:
				case CollisionType::SphereToAABB:
				case CollisionType::AABBToAABB:
				{
					
					if (glm::abs(massConstraint) > 0.0f)
					{
						stateA.LinearVelocity = stateA.LinearVelocity - collisionNormal * ((impulse == 0.0f ? 0.1f : impulse)) * stateB.InverseMass;
						stateB.LinearVelocity = stateB.LinearVelocity + collisionNormal * ((impulse == 0.0f ? 0.1f : impulse)) * stateA.InverseMass;

						stateA.AngularVelocity = stateA.AngularVelocity - stateA.InverseInertia * glm::cross(posA, collisionNormal * (impulse == 0.0f ? 0.1f : impulse));
						stateB.AngularVelocity = stateB.AngularVelocity + stateB.InverseInertia * glm::cross(posB, collisionNormal * (impulse == 0.0f ? 0.1f : impulse));
					}
					
					glm::vec3 tangent = dv - collisionNormal * glm::dot(dv, collisionNormal);
					float tangentLen = glm::length(tangent);

					if (tangentLen > 1e-6f)
					{
						tangent = tangent / tangentLen;

						float frictionalMass = (stateA.InverseMass + stateB.InverseMass) + glm::dot(tangent, glm::cross(stateA.InverseInertia * glm::cross(posA, tangent), posA) + glm::cross(stateB.InverseInertia * glm::cross(posB, tangent), posB));

						float frictionCoef = 0.1f;
						float friction = glm::dot(dv, tangent) * frictionCoef;

						stateA.LinearVelocity = stateA.LinearVelocity - tangent * (friction * stateA.InverseMass);
						stateB.LinearVelocity = stateB.LinearVelocity + tangent * (friction * stateB.InverseMass);

						stateA.AngularVelocity = stateA.AngularVelocity - stateA.InverseInertia * glm::cross(posA, tangent * friction);
						stateB.AngularVelocity = stateB.AngularVelocity + stateB.InverseInertia * glm::cross(posB, tangent * friction);
					}

					break;
				}

				case CollisionType::SphereToPlane:case CollisionType::AABBToPlane:case CollisionType::ObjectToPlane:
				{
					if (collider.GetType() == ColliderType::Plane)
					{
						if (stateB.Mass > 0.0f)
						{
							glm::vec3 planeNormal = collider.GetNormal();

							stateB.Mass = glm::clamp(stateA.Mass - stateB.Mass, stateB.Mass, 0.0f);

							stateB.LinearMomentum.x = planeNormal.x != 0.0f ? 0.0f : stateB.LinearMomentum.x;
							stateB.LinearMomentum.y = planeNormal.y != 0.0f ? 0.0f : stateB.LinearMomentum.y;
							stateB.LinearMomentum.z = planeNormal.z != 0.0f ? 0.0f : stateB.LinearMomentum.z;

							stateB.LinearVelocity.x = planeNormal.x != 0.0f ? 0.0f : stateB.LinearVelocity.x;
							stateB.LinearVelocity.y = planeNormal.y != 0.0f ? 0.0f : stateB.LinearVelocity.y;
							stateB.LinearVelocity.z = planeNormal.z != 0.0f ? 0.0f : stateB.LinearVelocity.z;

							stateB.Position.x = planeNormal.x != 0.0f ? planeNormal.x * impulse : stateB.Position.x;
							stateB.Position.y = planeNormal.y != 0.0f ? planeNormal.y * impulse : stateB.Position.y;
							stateB.Position.z = planeNormal.z != 0.0f ? planeNormal.z * impulse : stateB.Position.z;

							stateB.GravitationalPull = 0.0f;
						}
					}
					else
					{
						if (stateA.Mass > 0.0f)
						{
							glm::vec3 planeNormal = otherCollider.GetNormal();

							stateA.Mass = glm::clamp(stateB.Mass - stateA.Mass, stateA.Mass, 0.0f);

							stateA.LinearMomentum.x = planeNormal.x != 0.0f ? 0.0f : stateA.LinearMomentum.x;
							stateA.LinearMomentum.y = planeNormal.y != 0.0f ? 0.0f : stateA.LinearMomentum.y;
							stateA.LinearMomentum.z = planeNormal.z != 0.0f ? 0.0f : stateA.LinearMomentum.z;

							stateA.LinearVelocity.x = planeNormal.x != 0.0f ? 0.0f : stateA.LinearVelocity.x;
							stateA.LinearVelocity.y = planeNormal.y != 0.0f ? 0.0f : stateA.LinearVelocity.y;
							stateA.LinearVelocity.z = planeNormal.z != 0.0f ? 0.0f : stateA.LinearVelocity.z;

							stateA.Position.x = planeNormal.x != 0.0f ? planeNormal.x * impulse : stateA.Position.x;
							stateA.Position.y = planeNormal.y != 0.0f ? planeNormal.y * impulse : stateA.Position.y;
							stateA.Position.z = planeNormal.z != 0.0f ? planeNormal.z * impulse : stateA.Position.z;

							stateA.GravitationalPull = 0.0f;
						}
					}
					break;
				}
			}

			collider.SetState(stateA);
			otherCollider.SetState(stateB);
		}

	private:
		static Reference<PhysicsEngine> s_PhysicsEngine;

		std::unordered_map<std::string, ColliderComponent> m_Colliders;
		Sanguine::Timestep m_LastFrameTime{ 0.0 };
		bool isRunning = true;
	};
}