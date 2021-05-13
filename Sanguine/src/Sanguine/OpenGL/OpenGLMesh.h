#pragma once

#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLTransform.h"
#include "OpenGLVertexArray.h"

#include <glm/glm.hpp>

#include <assimp/scene.h>			// Output data structure
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/postprocess.h>		// Post processing flags

// GL_POINTS, GL_TRIANGLES, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY,
// GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY

namespace Sanguine
{
	class Texture2D;

	struct Material
	{
		Material()
		{

		}

		float Ambient = 0.1f;
		float Specular = 0.5f;

		glm::vec3 Color{ 1.0f };
		Sanguine::Reference<Sanguine::Texture2D> Texture;
	};

	class VertexData
	{
	public:
		VertexData()
		{
			BufferLayout =
			{
				{ ShaderDataType::Float3, "v_position"},
				{ ShaderDataType::Float3, "v_normal" },
				{ ShaderDataType::Float2, "v_texcoords" },
			};
		}

		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
		std::vector<glm::vec4> Colors;
		BufferLayout BufferLayout;

		std::vector<uint32_t> Indices;
		uint32_t VerticesCount = 0;
	};

	static const uint32_t INVALID_MATERIAL = 0xFFFFFFFF;

	class OpenGLMesh
	{
	public:
		OpenGLMesh();
		OpenGLMesh(const std::string& name);
		
		bool InitFromData(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices, uint32_t vertexCount);
		bool InitFromData(const std::string& filepath);
		bool InitFromScene(const aiScene pScene);

		void SetVertexData(VertexData data) { m_VertexData = data; }
		void SetDrawMode(GLenum primitive) { m_DrawMode = primitive; }

		void UpdateVertices(glm::mat3 model)
		{
			for (size_t i = 0; i < m_VertexData.Vertices.size(); i++)
			{
				m_VertexData.Vertices[i] = model * m_VertexData.Vertices[i];
			}
		}

		GLenum GetDrawMode() const { return m_DrawMode; }
		const std::string GetMeshID() const { return m_MeshID; }
		const VertexData GetVertexData() const { return m_VertexData; }
		const Reference<VertexArray> GetVertexArray() const { return m_VAO; }
		Reference<Material> GetMaterial() { return m_Material; }

		virtual ~OpenGLMesh();
		void ReleaseMemory();

	private:
		std::string m_MeshID;
		std::string fileLocation;

		GLenum m_DrawMode;
		VertexData m_VertexData;
		Reference<VertexArray> m_VAO;
		Reference<Material> m_Material;
	};
}