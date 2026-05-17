#pragma once

#include <EngineClasses/Assets/Material.h>

#include <vector>

struct sVertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

using std::vector;

class Mesh
{
public:
	vector<sVertex> mVertices;
	vector<unsigned int> mIndices;
	EngineAssets::Material* mMaterial;

	Mesh(vector<sVertex> vertices, vector<unsigned int> indices, EngineAssets::Material* material);
	void Draw();
private:
	unsigned int mVAO, mVBO, mEBO;

	void SetupMesh();
};