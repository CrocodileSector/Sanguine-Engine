#include "sgpch.h"
#include "Renderer2D.h"

#include "Shader.h"
#include "VertexArray.h"
#include "RendererCommand.h"

#include "Sanguine/OpenGL/OpenGLShader.h"

#define MAX_TEXURE_UNITS 32

namespace Sanguine
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float Tiles;
	};

	struct Renderer2DData
	{
		const uint32_t QuadsLimit = 10000;
		const uint32_t VerticesLimit = QuadsLimit * 4;
		const uint32_t IndicesLimit = QuadsLimit * 6;
		const uint32_t TextureSlotsLimit = MAX_TEXURE_UNITS;

		Reference<VertexArray> _VertexArray;
		Reference<VertexBuffer> _VertexBuffer;
		Reference<Shader>	_ColorShader;
		Reference<Shader> _TextureShader;
		Reference<Texture2D> _textureIndex;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBaseOffset = nullptr;
		QuadVertex* QuadVertexBufferCurrentOffset = nullptr;

		std::array<Reference<Texture2D>, MAX_TEXURE_UNITS> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data._VertexArray = VertexArray::Create();

		s_Data._VertexBuffer = VertexBuffer::Create(s_Data.VerticesLimit * sizeof(QuadVertex));
		s_Data._VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Float,  "a_Tiles" }
			});
		//s_Data._VertexArray->AddVertexBuffer(s_Data._VertexBuffer);

		s_Data.QuadVertexBufferBaseOffset = new QuadVertex[s_Data.VerticesLimit];

		uint32_t* indices = new uint32_t[s_Data.IndicesLimit];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.IndicesLimit; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data.IndicesLimit);
		s_Data._VertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		s_Data._textureIndex = Texture2D::Create(1, 1);
		uint32_t textureIndexData = 0xffffffff;
		s_Data._textureIndex->SetData(&textureIndexData, sizeof(uint32_t));

		int32_t samplers[MAX_TEXURE_UNITS];
		for (uint32_t i = 0; i < MAX_TEXURE_UNITS; ++i)
		{
			samplers[i] = i;
		}

		s_Data._TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data._TextureShader->Bind();
		s_Data._TextureShader->SetIntArray("u_Textures", samplers, MAX_TEXURE_UNITS);

		s_Data.TextureSlots[0] = s_Data._textureIndex;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data._TextureShader->Bind();
		s_Data._TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		SetupNewBatch();
	}

	void Renderer2D::SetupNewBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferCurrentOffset = s_Data.QuadVertexBufferBaseOffset;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferCurrentOffset - (uint8_t*)s_Data.QuadVertexBufferBaseOffset;
		s_Data._VertexBuffer->SetData(s_Data.QuadVertexBufferBaseOffset, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
		{
		}

		RendererCommand::DrawIndexed(s_Data._VertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount >= s_Data.VerticesLimit)
		{
			EndScene();
			SetupNewBatch();
		}

		float textureIndex = 0.0f;
		float tiles = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D> texture, float tiles, const glm::vec4& tint)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, tiles, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D> texture, float tiles, const glm::vec4& tint)
	{
		if (s_Data.QuadIndexCount >= s_Data.VerticesLimit)
		{
			EndScene();
			SetupNewBatch();
		}

		const glm::vec4 color = tint;
		
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount >= s_Data.VerticesLimit)
		{
			EndScene();
			SetupNewBatch();
		}

		float textureIndex = 0.0f;
		float tiles = 0.0f;
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Reference<Texture2D> texture, float tiles, const glm::vec4& tint)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiles, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Reference<Texture2D> texture, float tiles, const glm::vec4& tint)
	{
		if (s_Data.QuadIndexCount >= s_Data.VerticesLimit)
		{
			EndScene();
			SetupNewBatch();
		}

		const glm::vec4 color = tint;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadVertexBufferCurrentOffset->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferCurrentOffset->Color = color;
		s_Data.QuadVertexBufferCurrentOffset->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferCurrentOffset->TexIndex = textureIndex;
		s_Data.QuadVertexBufferCurrentOffset->Tiles = tiles;
		s_Data.QuadVertexBufferCurrentOffset++;

		s_Data.QuadIndexCount += 6;
	}
}