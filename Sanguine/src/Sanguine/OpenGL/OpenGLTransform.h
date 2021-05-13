#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Sanguine
{
    class Transform3D
    {
    public:
        Transform3D() :
            m_Translation(1.0f),
            m_Rotation(1.0f),
            m_Scale(1.0f),
            m_Model(1.0f)
        {

        }

        inline glm::mat4& Translate(const glm::vec3& offset)
        {
            m_Translation = glm::translate(offset);
            return m_Translation;
        }

        inline glm::mat4& RotateOY(float radians)
        {
            m_Rotation = glm::rotate(m_Rotation, radians, glm::vec3{ 0.0f, 1.0f, 0.0f });
            return m_Rotation;
        }

        inline glm::mat4& RotateOX(float radians)
        {
            m_Rotation = glm::rotate(m_Rotation, radians, glm::vec3{ 1.0f, 0.0f, 0.0f });
            return m_Rotation;
        }

        inline glm::mat4& RotateOZ(float radians)
        {
            m_Rotation = glm::rotate(m_Rotation, radians, glm::vec3{ 0.0f, 0.0f, 1.0f });
            return m_Rotation;
        }

        inline glm::mat4& SetScale(const glm::vec3& scale)
        {
            m_Scale = glm::scale(scale);
            return m_Scale;
        }

        inline glm::mat4 GetModel() 
        {
            m_Model = m_Translation * m_Rotation * glm::mat4(1.0f) * m_Scale;

            return m_Model;
        }

        inline glm::mat4& GetTranslation() { return m_Translation; }
        inline glm::mat4& GetRotation() { return m_Rotation; }
        inline glm::mat4& GetScale()    { return m_Scale;    }

        inline void SetTranslation(glm::mat4& translation) { m_Translation = translation; }
        inline void SetRotation(glm::mat4& rotation) { m_Rotation = rotation; }

        inline void SetModel(const glm::mat4& model) { m_Model = model; }

    private:
        glm::mat4 m_Model;
        glm::mat4 m_Translation;
        glm::mat4 m_Rotation;
        glm::mat4 m_Scale;
    };
}