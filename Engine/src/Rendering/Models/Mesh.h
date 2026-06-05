#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <EngineClasses/Assets/Material.h>

#include <vector>

struct sVertex {
	Refraction::Math::Vector3 pos;
	Refraction::Math::Vector3 normal;
	Refraction::Math::Vector2 texCoord;
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