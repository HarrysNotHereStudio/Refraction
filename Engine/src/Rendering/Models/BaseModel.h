#pragma once

#include <Core/Log.h>
#include <Core/Utilities.h>
#include <EngineClasses/Assets/Shader.h>
#include <EngineClasses/Assets/Texture.h>
#include <Math/Transform.h>
#include "Mesh.h"

//TODO: implement native file loading, assimp can't stop crying about linking
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class BaseModel
{
public:
	Refraction::Math::Transform* mTransform;
	EngineAssets::Shader* mShader;
	std::string mSourcePath;

	BaseModel() = default;
	BaseModel(std::string modelSourcePath);

	virtual void DrawModel();

protected:
	void DrawMeshesRaw();

private:
	std::vector<Mesh> mMeshes;
	std::vector<EngineAssets::Texture*> mTextures;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<EngineAssets::Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};