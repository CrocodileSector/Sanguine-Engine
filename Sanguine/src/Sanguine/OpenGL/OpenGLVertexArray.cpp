#include "sgpch.h"
#include "OpenGLVertexArray.h"

namespace Sanguine
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ReleaseMemory();
	}

	void OpenGLVertexArray::Bind() 
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind()
	{
		m_VertexBuffer->Unbind();
		m_VertexBuffer.reset();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::ReleaseMemory()
	{
		if (m_VertexBuffer) 
		{
			m_VertexBuffer->Unbind();
			m_VertexBuffer.reset();
		}

		if (m_RendererID)
		{
			glDeleteVertexArrays(1, &m_RendererID);
		}
	}

	void OpenGLVertexArray::SetVertexBuffer(const Reference<VertexBuffer>& vertexBuffer)
	{
		SG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffer = vertexBuffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;

		Bind();
		m_IndexBuffer->Bind();
	}
}