#include "sgpch.h"
#include "OpenGLMesh.h"

#include "Sanguine/Core/Core.h"
#include "Sanguine/Renderer/Buffer.h"
#include "Sanguine/Renderer/TextureManager.h"

namespace Sanguine
{
	const uint32_t kMaxMeshes = 100000;
	const uint32_t kMaxIndices = kMaxMeshes * 6;
	const uint32_t kMaxVertices = kMaxMeshes * 4;

	OpenGLMesh::OpenGLMesh()
		: m_MeshID(std::string()), m_DrawMode(GL_TRIANGLES)
	{
		m_Material = CreateReference<Material>();
	}

	OpenGLMesh::OpenGLMesh(const std::string& name) 
		: m_MeshID(name), m_DrawMode(GL_TRIANGLES)
	{
		m_Material = CreateReference<Material>();
	}

	OpenGLMesh::~OpenGLMesh()
	{
		ReleaseMemory();
	}

	void OpenGLMesh::ReleaseMemory()
	{
		m_VertexData.Indices.clear();

		m_VAO != nullptr ? m_VAO->ReleaseMemory() : false;
		m_VAO != nullptr ? m_VAO.reset() : false;

		m_Material->Texture != nullptr ? m_Material->Texture.reset() : false;
		m_Material != nullptr ? m_Material.reset() : false;
	}

	bool OpenGLMesh::InitFromData(const std::string& fileLocation)
	{
		this->fileLocation = fileLocation;
		std::string file = fileLocation.c_str();

		Assimp::Importer Importer;

		unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
		if (m_DrawMode == GL_TRIANGLES) flags |= aiProcess_Triangulate;

		return InitFromScene(*Importer.ReadFile(file, 0));

		// pScene is freed when returning because of Importer
		SG_CORE_ERROR("Error parsing '%s': '%s'\n", file.c_str(), Importer.GetErrorString());
		return false;
	}

	bool OpenGLMesh::InitFromData(const std::vector<uint32_t>& indices, const std::vector<glm::vec3>& vertices, uint32_t vertexCount)
	{
		m_VertexData.Vertices = vertices;
		m_VertexData.Indices = indices;
		m_VertexData.VerticesCount = vertexCount;

		uint32_t indicesCount = m_VertexData.Indices.size();
		uint32_t* indicesBuffer = new uint32_t[indicesCount];
		for (int i = 0; i < indicesCount; i++)
		{
			indicesBuffer[i] = m_VertexData.Indices[i];
		}

		m_VAO = VertexArray::Create();
		
		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(indicesBuffer, indicesCount);
		m_VAO->SetIndexBuffer(indexBuffer);

		return m_VAO->GetID() != 0;
	}

	bool OpenGLMesh::InitFromScene(const aiScene pScene)
	{
		m_VAO = VertexArray::Create();

		// Count the number of vertices and indices
		for (unsigned int i = 0; i < pScene.mNumMeshes; i++)
		{
			m_VertexData.VerticesCount += pScene.mMeshes[i]->mNumVertices;

			// Populate the vertex attribute vectors
			for (unsigned int j = 0; j < pScene.mMeshes[i]->mNumVertices; j++)
			{
				const aiVector3D pPos = pScene.mMeshes[i]->mVertices[j];
				const aiVector3D pNormal = pScene.mMeshes[i]->mNormals[j];
				const aiVector3D pTexCoord = pScene.mMeshes[i]->HasTextureCoords(0) ? pScene.mMeshes[i]->mTextureCoords[0][j] : aiVector3D { 0.0f };

				m_VertexData.Vertices.push_back(glm::vec3(pPos.x, pPos.y, pPos.z));
				m_VertexData.Normals.push_back(glm::vec3(pNormal.x, pNormal.y, pNormal.z));
				m_VertexData.TexCoords.push_back(glm::vec2(pTexCoord.x, pTexCoord.y));
				m_VertexData.Colors.push_back(glm::vec4(m_Material->Color, 1.0f));
			}

			// Init the index buffer
			for (unsigned int j = 0; j < pScene.mMeshes[i]->mNumFaces; j++)
			{
				m_VertexData.Indices.push_back(pScene.mMeshes[i]->mFaces[j].mIndices[0]);
				m_VertexData.Indices.push_back(pScene.mMeshes[i]->mFaces[j].mIndices[1]);
				m_VertexData.Indices.push_back(pScene.mMeshes[i]->mFaces[j].mIndices[2]);

				if (pScene.mMeshes[i]->mFaces[j].mNumIndices == 4)
				{
					m_VertexData.Indices.push_back(pScene.mMeshes[i]->mFaces[j].mIndices[3]);
				}
			}
		}

		uint32_t indicesCount = m_VertexData.Indices.size();
		uint32_t* indices = new uint32_t[indicesCount];
		for (int i = indicesCount - 1; i >= 0; --i)
		{
			indices[i] = m_VertexData.Indices[i];
		}

		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indicesCount);
		m_VAO->SetIndexBuffer(indexBuffer);

		return m_VAO->GetID() != 0;
	}
}