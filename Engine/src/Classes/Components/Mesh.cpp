#include <json.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Core/Common.h>
#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>

#include "Mesh.h"

namespace Refraction::Components {
	int Mesh::FrameMeshCount = 0;
	int Mesh::FrameVertexCount = 0;


	MeshFragment::MeshFragment(std::vector<sVertex> vertices, std::vector<unsigned int> indices, Assets::Material* material) {
		mVertices = vertices;
		mIndices = indices;
		mMaterial = material;

		SetupMesh();
	}

	void MeshFragment::Draw() {
		mMaterial->Activate();
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void MeshFragment::SetupMesh() {
		// Create buffers
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(sVertex), &mVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		// Load vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, texCoord));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}


	void ProcessNode(std::string sourcePath, std::vector<Assets::Texture*>& textureArray, std::vector<MeshFragment*>& fragments, aiNode* node, const aiScene* scene);
	MeshFragment* ProcessMesh(std::string& sourcePath, std::vector<Assets::Texture*>& textureArray, aiMesh* mesh, const aiScene* scene);
	std::vector<Assets::Texture*> LoadMaterialTextures(std::string& sourcePath, std::vector<Assets::Texture*>& textureArray, aiMaterial* mat, aiTextureType type, std::string typeName);

	static void ProcessNode(std::string sourcePath, std::vector<Assets::Texture*>& textureArray, std::vector<MeshFragment*>& fragments, aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			fragments.push_back(ProcessMesh(sourcePath, textureArray, mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(sourcePath, textureArray, fragments, node->mChildren[i], scene);
		}
	}

	static MeshFragment* ProcessMesh(std::string& sourcePath, std::vector<Assets::Texture*>& textureArray, aiMesh* mesh, const aiScene* scene) {
		std::vector<sVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Assets::Texture*> diffuseMaps;
		std::vector<Assets::Texture*> specularMaps;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			sVertex vertex;

			aiVector3D importPos = mesh->mVertices[i];
			vertex.pos = Math::Vector3(importPos.x, importPos.y, importPos.z);

			aiVector3D importNormal = mesh->mNormals[i];
			vertex.normal = Math::Vector3(importNormal.x, importNormal.y, importNormal.z);

			if (mesh->HasTextureCoords(0)) {
				aiVector3D importTexCoord = mesh->mTextureCoords[0][i];
				vertex.texCoord = Math::Vector2(importTexCoord.x, importTexCoord.y);
			} else
				vertex.texCoord = Math::Vector2(0.0f);

			vertices.push_back(vertex);
		}


		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			diffuseMaps = LoadMaterialTextures(sourcePath, textureArray, material, aiTextureType_DIFFUSE, REFRACT_TEXTURE_TYPE_DIFFUSE);
			specularMaps = LoadMaterialTextures(sourcePath, textureArray, material, aiTextureType_SPECULAR, REFRACT_TEXTURE_TYPE_SPECULAR);
		}

		Assets::Material* newMat = new Assets::Material();
		if (diffuseMaps.size() > 0) newMat->mDiffuse = diffuseMaps[0];
		else Log::SWarn("Imported mesh does not contain any diffuse textures. There may be undefined behaviour.");
		if (specularMaps.size() > 0) newMat->mSpecular = specularMaps[0];
		else Log::SWarn("Imported mesh does not contain any specular textures. There may be undefined behaviour.");
		newMat->mShader = Assets::Shader::GetShaderByName("gbufferShader");

		return new MeshFragment(vertices, indices, newMat);
	}

	static std::vector<Assets::Texture*> LoadMaterialTextures(std::string& sourcePath, std::vector<Assets::Texture*>& textureArray, aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Assets::Texture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			std::string fullPath = sourcePath + "/" + str.C_Str();

			bool skip = false;
			for (unsigned int j = 0; j < textureArray.size(); j++) {
				if (std::strcmp(textureArray[j]->GetMetadata().SourcePath.string().data(), fullPath.data()) == 0) {
					textures.push_back(textureArray[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				auto texture = Assets::Texture::GetTexture(fullPath, typeName);
				textures.push_back(texture);
				textureArray.push_back(texture);
			}
		}
		return textures;
	}



	Mesh::Mesh() {
		mDisplayName = "MeshComponent";
		mTransform = Math::Transform();
	}

	void Mesh::Render() {
		auto shader = Assets::Shader::GetShaderByName("gbufferShader");
		shader->Activate();
		shader->SetUniformMat4("modelTransform", mParent->mTransform.GetTransform());
		for (auto& mesh : mFragments) {
			mesh->Draw();
			FrameVertexCount += (int)mesh->mVertices.size();
			FrameMeshCount++;
		}
	}

	std::string Mesh::Serialise() {
		using nlohmann::json;

		try {
			json serialised = json::parse(AComponent::Serialise());
			serialised["Transform"] = Utilities::ClassSerialiser::Serialise(mTransform);
			serialised["MeshAssetPath"] = mSourcePath.string();
			return serialised.dump();
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON: " + std::string(err.what()));
		}
	}

	void Mesh::Deserialise(std::string serialised) {
		using nlohmann::json;

		try {
			AComponent::Deserialise(serialised);
			json data = json::parse(serialised);
			mTransform = Utilities::ClassSerialiser::DeserialiseTransform(data.at("Transform"));
			LoadModel(std::filesystem::path(data.at("MeshAssetPath").get<std::string>()));
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON: " + std::string(err.what()));
		}
	}

	void Mesh::LoadModel(std::filesystem::path path) {
		auto pathStr = path.string();
		if (!std::filesystem::exists(path)) throw std::runtime_error("Could not find source file " + pathStr);
		Log::SInfo("Loading mesh " + pathStr);
		mSourcePath = path;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(pathStr, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			Log::SError("MODEL LOAD FAILED | " + std::string(import.GetErrorString()));
			return;
		}
		Log::SInfo("Parsing mesh scene data...");
		ProcessNode(mSourcePath.string().substr(0, pathStr.find_last_of("/")), mTextures, mFragments, scene->mRootNode, scene);
	}
}
