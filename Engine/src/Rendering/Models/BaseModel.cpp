#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../ShaderManager.h"

#include "BaseModel.h"

namespace RMath = Refraction::Math;
using RMath::Transform;
using RMath::Vector2;
using RMath::Vector3;

BaseModel::BaseModel(std::string modelSourcePath) {
	mTransform = new Transform();
	mShader = ShaderManager::GetShaderByName("gbufferShader");
	LoadModel(modelSourcePath);
};

void BaseModel::DrawModel() {
	mShader->Activate();
	mShader->SetUniformMat4("modelTransform", mTransform->GetTransform());

	DrawMeshesRaw();
	//Log::Info("Drawn for model" + mSourcePath);
}

void BaseModel::LoadModel(std::string path) {
	Log::Info("Loading model " + path);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Log::Info("MODEL LOAD FAILED | " + std::string(import.GetErrorString()));
		return;
	}
	mSourcePath = path.substr(0, path.find_last_of("/"));
	Log::Info("Parsing scene data...");
	ProcessNode(scene->mRootNode, scene);
}

void BaseModel::ProcessNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh BaseModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<sVertex> vertices;
	std::vector<unsigned int> indices;
	vector<EngineAssets::Texture*> diffuseMaps;
	vector<EngineAssets::Texture*> specularMaps;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		sVertex vertex;

		aiVector3D importPos = mesh->mVertices[i];
		vertex.pos = Vector3(importPos.x, importPos.y, importPos.z);

		aiVector3D importNormal = mesh->mNormals[i];
		vertex.normal = Vector3(importNormal.x, importNormal.y, importNormal.z);

		if (mesh->HasTextureCoords(0)) {
			aiVector3D importTexCoord = mesh->mTextureCoords[0][i];
			vertex.texCoord = Vector2(importTexCoord.x, importTexCoord.y);
		} else
			vertex.texCoord = Vector2(0.0f);

		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, REFRACT_TEXTURE_TYPE_DIFFUSE);
		specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, REFRACT_TEXTURE_TYPE_SPECULAR);
	}

	EngineAssets::Material* newMat = new EngineAssets::Material();
	if (diffuseMaps.size() > 0) newMat->mDiffuse = diffuseMaps[0];
	else Log::Warn("Imported mesh does not contain any diffuse textures. There may be undefined behaviour.");
	if (specularMaps.size() > 0) newMat->mSpecular = specularMaps[0];
	else Log::Warn("Imported mesh does not contain any specular textures. There may be undefined behaviour.");
	newMat->mShader = mShader;

	return Mesh(vertices, indices, newMat);
}

vector<EngineAssets::Texture*> BaseModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	vector<EngineAssets::Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string fullPath = mSourcePath + "/" + str.C_Str();

		bool skip = false;
		for (unsigned int j = 0; j < mTextures.size(); j++) {
			if (std::strcmp(mTextures[j]->GetSourcePath().data(), fullPath.data()) == 0) {
				textures.push_back(mTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			EngineAssets::Texture* texture = EngineAssets::Texture::GetTexture(fullPath, typeName);
			textures.push_back(texture);
			mTextures.push_back(texture);
		}
	}
	return textures;
}

void BaseModel::DrawMeshesRaw() {
	for (auto& mesh : mMeshes)
		mesh.Draw();
}
