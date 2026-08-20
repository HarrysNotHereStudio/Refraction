#include <json.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Core/Common.h>
#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>

#include "Mesh.h"

namespace Refraction::Components {
	using Engine::Platform::AMeshFragment;

	int Mesh::FrameMeshCount = 0;
	int Mesh::FrameVertexCount = 0;

	void ProcessNode(std::string sourcePath, std::vector<Common::Ref<Assets::Material>>& materials, std::vector<Common::Ref<AMeshFragment>>& fragments, aiNode* node, const aiScene* scene);
	Common::Ref<AMeshFragment> ProcessMesh(std::string& sourcePath, std::vector<Common::Ref<Assets::Material>>& materials, aiMesh* mesh, const aiScene* scene);
	std::vector<Common::Ref<Assets::Image>> LoadMaterialTextures(std::string& sourcePath, aiMaterial* mat, aiTextureType type, std::string typeName);

	static void ProcessNode(std::string sourcePath, std::vector<Common::Ref<Assets::Material>>& materials, std::vector<Common::Ref<AMeshFragment>>& fragments, aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			fragments.push_back(ProcessMesh(sourcePath, materials, mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(sourcePath, materials, fragments, node->mChildren[i], scene);
		}
	}

	static Common::Ref<AMeshFragment> ProcessMesh(std::string& sourcePath, std::vector<Common::Ref<Assets::Material>>& materials, aiMesh* mesh, const aiScene* scene) {
		std::vector<Engine::sVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Common::Ref<Assets::Image>> diffuseMaps;
		std::vector<Common::Ref<Assets::Image>> specularMaps;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Engine::sVertex vertex;

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

		return AMeshFragment::MakeMeshFragment(vertices, indices, materials[mesh->mMaterialIndex]);
	}

	static std::vector<Common::Ref<Assets::Image>> LoadMaterialTextures(std::string& sourcePath, aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Common::Ref<Assets::Image>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			std::string fullPath = sourcePath + "/" + str.C_Str();

			auto texture = Assets::Image::FromPath(fullPath);
			textures.push_back(texture);
		}
		return textures;
	}



	Mesh::Mesh() {
		mClassName = "MeshComponent";
		mTransform = Math::Transform();
	}

	void Mesh::Render() {
		auto shader = Assets::Shader::GetShaderByName("gbufferShader");
		if (!shader.Valid()) return;
		shader.Get()->Activate();
		shader.Get()->SetUniformMat4("modelTransform", mTransform.ToMatrix() * mParent->GetWorldTransform().ToMatrix());
		for (auto& mesh : mFragments) {
			mesh->Draw();
			FrameVertexCount += (int)mesh->mVertices.size();
			FrameMeshCount++;
		}
	}

	std::string Mesh::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AComponent::Serialise(), [&](nlohmann::json& json) {
			json["Transform"] = Utilities::ClassSerialiser::Serialise(mTransform);
			json["MeshAssetPath"] = mSourcePath.string();
		});
	}

	void Mesh::Deserialise(std::string serialised) {
		AComponent::Deserialise(serialised);
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mTransform = Utilities::ClassSerialiser::DeserialiseTransform(json.at("Transform"));
			LoadModel(std::filesystem::path(json.at("MeshAssetPath").get<std::string>()));
		});
	}

	void Mesh::LoadModel(std::filesystem::path path) {
		auto pathStr = path.string();
		if (!std::filesystem::exists(path)) {
			Log::SWarn("Could not find source file " + pathStr + ", aborting loading for this mesh");
			return;
		}
		Log::SInfo("Loading mesh " + pathStr);
		mSourcePath = path;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(pathStr, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			Log::SError("MODEL LOAD FAILED | " + std::string(import.GetErrorString()));
			return;
		}

		auto importSourcePath = mSourcePath.string().substr(0, pathStr.find_last_of("/"));

		// Create materials
		Log::SInfo("Parsing materials...");
		if (scene->mNumMaterials > 0) {
			for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
				auto importMat = scene->mMaterials[i];
				auto mat = new Assets::Material();

				auto diffuseMaps = LoadMaterialTextures(importSourcePath, importMat, aiTextureType_DIFFUSE, RFCT_TEXTURE_TYPE_DIFFUSE);
				if (diffuseMaps.size() > 0) mat->mDiffuse = diffuseMaps[0];
				else {
					Log::SWarn("Imported material does not associate with any diffuse textures, using default texture.");
					mat->mDiffuse = Assets::Image::FromPath(FileHandling::GetResourcesPath() / "textures" / "Basic.png");
				}
				auto specularMaps = LoadMaterialTextures(importSourcePath, importMat, aiTextureType_SPECULAR, RFCT_TEXTURE_TYPE_SPECULAR);
				if (specularMaps.size() > 0) mat->mSpecular = specularMaps[0];
				else {
					Log::SWarn("Imported material does not associate with any specular textures, using default texture.");
					mat->mSpecular = Assets::Image::FromPath(FileHandling::GetResourcesPath() / "textures" / "Basic.png");
				}

				mat->mShader = Assets::Shader::GetShaderByName("gbufferShader");
				mMaterials.push_back(mat);
			}
		} else {
			auto mat = new Assets::Material();
			mat->mDiffuse = Assets::Image::FromPath(FileHandling::GetResourcesPath() / "textures" / "Basic.png");
			mat->mSpecular = mat->mDiffuse;

			mat->mShader = Assets::Shader::GetShaderByName("gbufferShader");
			mMaterials.push_back(mat);
		}

		// Load meshes
		Log::SInfo("Parsing mesh data...");
		ProcessNode(importSourcePath, mMaterials, mFragments, scene->mRootNode, scene);
	}
}
