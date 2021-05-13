#pragma once

#include <limits>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Sanguine
{
	class OpenGLMesh;

	enum class CollisionType
	{
		SphereToSphere = 0,
		SphereToAABB,
		SphereToPlane,
		AABBToPlane,
		AABBToAABB,
		ObjectToPlane,
		ObjectToSphere,
		ObjectToAABB,
		None
	};

	class CollisionData
	{
	public:
		CollisionData()
		{

		}

		CollisionData(const bool doesCollide, const glm::vec3& direction, const CollisionType type) :
			m_Collides(doesCollide),
			m_Direction(direction),
			m_Type(type)
		{

		}

		CollisionData& operator = (const CollisionData& other)
		{
			m_Collides = other.m_Collides;
			m_Direction = other.m_Direction;
			m_Type = other.m_Type;

			return (*this);
		}

		inline const CollisionType GetType()			const { return m_Type; }
		inline bool DoesCollide()						const { return m_Collides; }
		inline const glm::vec3& GetDirection()			const { return m_Direction; }
		inline float GetDistanceMagnitude()				const { return std::sqrt(glm::dot(m_Direction, m_Direction)); }
		
	private:
		bool  m_Collides;
		glm::vec3 m_Direction;
		CollisionType m_Type;
	};

	struct ColliderState
	{
		glm::quat Orientation, Spin;
		glm::vec3 Position, LastPosition;

		glm::vec3 LinearVelocity, AngularVelocity;
		glm::vec3 LinearMomentum, AngularMomentum;

		glm::vec3 LinearOffset;

		float Mass, InverseMass, Inertia, InverseInertia, GravitationalPull;

		float Delta = 0.0f, Accumulator = 0.0f, DeltaLimit = 30.0f;

		ColliderState(float mass = 1.0f, 
					  float inertia = 1.0f, 
					  const glm::vec3& position = glm::vec3(0.0f), 
					  const glm::vec3& linearMomentum = glm::vec3{0.0f, 0.1f, 0.0f}, 
					  const glm::vec3& angularMomentum = glm::vec3(0.0f), 
					  const glm::vec3& linearVelocity = glm::vec3{ 0.0f, 0.1f, 0.0f },
					  const glm::vec3& angularVelocity = glm::vec3(0.0f), 
					  const glm::quat& spin = glm::quat(glm::vec3(0.0f)), 
					  const glm::quat& orientation = glm::quat(glm::vec3(1.0f))) : 
		   Spin(spin),
		   Mass(mass),
		   Inertia(inertia),
		   Position(position),
		   LastPosition(position),
		   Orientation(orientation),
		   LinearMomentum(linearMomentum),
		   AngularMomentum(angularMomentum),
		   LinearVelocity(linearVelocity),
		   AngularVelocity(angularVelocity),
		   InverseMass(Mass * -1.0f),
		   InverseInertia(Inertia * -1.0f),
		   GravitationalPull(InverseMass * 0.1f),
		   LinearOffset(glm::vec3{0.f})
		{

		}
	};

	struct Derivative
	{
		glm::vec3 velocity{ 1.0f };
		glm::vec3 force{ 1.0f };

		Derivative()
		{

		}
	};

	enum class ColliderType
	{
		Sphere,
		AABB,
		Plane,
		Object,
		None
	};

	class ColliderComponent
	{
	public:
		ColliderComponent() = default;
		ColliderComponent(const ColliderComponent&) = default;

		ColliderComponent(const ColliderState state, ColliderType type, bool isRemote = false) : m_State(state), m_ColliderType(type), m_IsRemote(isRemote)
		{

		}

		~ColliderComponent()
		{
			m_Mesh != nullptr ? m_Mesh.reset() : false;
		}

		void Update(float delta)
		{
			m_State.AngularVelocity = m_State.AngularMomentum * m_State.InverseInertia;

			m_State.Orientation = glm::normalize(m_State.Orientation);

			glm::quat q(m_State.AngularVelocity.x, m_State.AngularVelocity.y, m_State.AngularVelocity.z, 0.0f);

			m_State.Spin = 0.5f * q * m_State.Orientation;

			m_State.LastPosition = m_State.Position;

			m_State.LinearMomentum.y += m_State.GravitationalPull * m_State.InverseMass;
			m_State.LinearVelocity += (m_State.LinearMomentum * m_State.InverseMass) * delta;

			m_State.Position += m_State.LinearVelocity * delta;

			if (m_State.Mass <= 0.0f && m_State.Position.y <= 1.0f)
			{
				m_State.Position.y = 1.0f;
				m_State.LinearMomentum.y = 0.0f;
				m_State.LinearVelocity.y = 0.0f;
			}

			m_State.LinearMomentum = glm::mix(m_State.LinearMomentum, glm::vec3(0.0f), delta);
			m_State.LinearVelocity = glm::mix(m_State.LinearVelocity, glm::vec3(0.0f), delta);

			m_State.LinearOffset = m_State.Position - m_State.LastPosition;
			Transform(m_State.LinearOffset);
		}

		inline bool IsRemoteView() const { return m_IsRemote; }

		inline ColliderType& GetType() { return m_ColliderType; }
		inline void SetType(const ColliderType& type) { m_ColliderType = type; }

		inline ColliderState& GetState() { return m_State; }
		inline void SetState(const ColliderState& state) { m_State = state; }

		inline Reference<OpenGLMesh> GetMesh() { return m_Mesh; }
		inline void SetMesh(Reference<OpenGLMesh> mesh) { m_Mesh = mesh; }

		void Transform(const glm::vec3& offset)
		{
			if (m_ColliderType == ColliderType::Sphere)
			{
				m_Center += offset;
			}

			if (m_ColliderType == ColliderType::AABB)
			{
				m_MinExtents.x += offset.x;
				m_MinExtents.y += offset.y;
				m_MinExtents.z += offset.z;

				m_MaxExtents.x += offset.x;
				m_MaxExtents.y += offset.y;
				m_MaxExtents.z += offset.z;
			}
		}

		//Sphere Accessors
		void SetCenter(glm::vec3 center)
		{
			if (m_ColliderType == ColliderType::Sphere) { m_Center = center; }
		}

		glm::vec3 GetCenter()
		{
			if (m_ColliderType == ColliderType::Sphere)
			{
				return m_Center;
			}

			if (m_ColliderType == ColliderType::AABB)
			{
				float centerX = (m_MinExtents.x + m_MaxExtents.x) / 2.0f;
				float centerY = (m_MinExtents.y + m_MaxExtents.y) / 2.0f;
				float centerZ = (m_MinExtents.z + m_MaxExtents.z) / 2.0f;

				return glm::vec3(centerX, centerY, centerZ);
			}
		}

		//Plane Accessors
		void SetRadius(float radius) { m_Radius = radius; }
		float GetRadius() { return m_Radius; }

		inline const glm::vec3& GetNormal() const { return m_Normal; }
		inline const void SetNormal(const glm::vec3& normal) { m_Normal = normal; }

		inline float GetDistance() const { return m_Distance; }
		inline const void SetDistance(const float distance) { m_Distance = distance; }

		//AABB Accessors
		inline const void CalculateExtents()
		{
			m_MinExtents = glm::vec3{ m_State.Position.x - defaultWidth / 2, m_State.Position.y - defaultHeight / 2, m_State.Position.z - defaultDepth / 2 };
			m_MaxExtents = glm::vec3{ m_State.Position.x + defaultWidth / 2, m_State.Position.y + defaultHeight / 2, m_State.Position.z + defaultDepth / 2 };
		}
		inline const glm::vec3 GetMinExtents() const { return m_MinExtents; }
		inline const glm::vec3 GetMaxExtents() const { return m_MaxExtents; }
		inline const void SetMinExtents(const glm::vec3 extent) { m_MinExtents = extent; }
		inline const void SetMaxExtents(const glm::vec3 extent) { m_MaxExtents = extent; }

	protected:
		bool m_IsRemote = false;

		float t = 1.0f;
		ColliderType m_ColliderType;
		ColliderState m_State, m_PreviousState;

		Reference<OpenGLMesh> m_Mesh;

		//Plane properties
		glm::vec3 m_Normal;
		float	m_Distance;

		//BSphere properties
		glm::vec3 m_Center;
		float	  m_Radius;

		//AABB properties
		glm::vec3 m_MinExtents, m_MaxExtents;
		uint32_t defaultWidth = 2, defaultHeight = 2, defaultDepth = 2;
	};
}