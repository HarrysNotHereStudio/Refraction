#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Utilities.h"
#include "../Graphics/BaseShader.h"
#include "../Graphics/BaseTexture.h"
#include "Transform.h"
#include "Mesh.h"

class BaseModel
{
public:
	Transform* m_pTransform;
	BaseShader* m_pShader;
	std::string sourcePath;

	BaseModel() = default;
	BaseModel(std::string modelSourcePath) {
		m_pTransform = new Transform();
		m_pShader = new BaseShader();
		loadModel(modelSourcePath);
	};

	void DrawModel();
private:
	std::vector<Mesh> m_meshes;
	std::vector<BaseTexture> m_textures;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<BaseTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};