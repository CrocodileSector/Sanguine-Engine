#pragma once

#include "Sanguine/Renderer/VertexArray.h"

namespace Sanguine
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:					return GL_FLOAT;
		case ShaderDataType::Float2:				return GL_FLOAT;
		case ShaderDataType::Float3:				return GL_FLOAT;
		case ShaderDataType::Float4:				return GL_FLOAT;

		case ShaderDataType::Int:					return GL_INT;
		case ShaderDataType::Int2:					return GL_INT;
		case ShaderDataType::Int3:					return GL_INT;
		case ShaderDataType::Int4:					return GL_INT;

		case ShaderDataType::Mat3:					return GL_FLOAT;
		case ShaderDataType::Mat4:					return GL_FLOAT;

		case ShaderDataType::Bool:					return GL_BOOL;
		}

		SG_CORE_ASSERT(false, "Unknow Shader data type!");
		return 0;
	}

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) override;
		virtual void SetID(uint32_t newID) override { m_RendererID = newID; }

		virtual const Reference<VertexBuffer>& GetVertexBuffer() const override { return m_VertexBuffer; }
		virtual const Reference<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		virtual const uint32_t& GetID() const override { return m_RendererID; }

		virtual void ReleaseMemory() override;

	private:
		uint32_t m_RendererID;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;
	};
}