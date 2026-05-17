#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>

#include <EngineLog.h>
#include <EngineUtilities.h>
#include <EngineClasses/Assets/Shader.h>
#include <EngineClasses/Assets/Texture.h>
#include "Transform.h"
#include "Mesh.h"

class BaseModel
{
public:
	Transform* mTransform;
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
	vector<EngineAssets::Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};