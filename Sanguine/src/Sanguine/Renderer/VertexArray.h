#pragma once

#include "Sanguine/Core/Core.h"
#include "Sanguine/Renderer/Buffer.h"

namespace Sanguine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) = 0; 
		virtual void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) = 0;
		virtual void SetID(uint32_t newID) = 0;

		virtual const Reference<VertexBuffer>& GetVertexBuffer() const = 0;
		virtual const Reference<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const uint32_t& GetID() const = 0;

		virtual void ReleaseMemory() = 0;

		static Reference<VertexArray> Create();
	};
}