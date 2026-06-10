#pragma once

#include <vector>

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Transform.h>
#include <EngineClasses/Assets/Material.h>
#include <EngineClasses/Assets/Texture.h>

#include "AComponent.h"

struct sVertex {
	Refraction::Math::Vector3 pos;
	Refraction::Math::Vector3 normal;
	Refraction::Math::Vector2 texCoord;
};


namespace {
	class MeshFragment {
	public:
		std::vector<sVertex> mVertices;
		std::vector<unsigned int> mIndices;
		Refraction::Assets::Material* mMaterial;

		MeshFragment(std::vector<sVertex> vertices, std::vector<unsigned int> indices, Refraction::Assets::Material* material);

		void Draw();
	private:
		unsigned int mVAO, mVBO, mEBO;

		void SetupMesh();
	};
}

namespace Refraction::Components {
	class Mesh : public AComponent {
	public:
		Math::Transform mTransform;

		Mesh();

		void LoadModel(std::string path);
		void Tick(float delta) override {};
		void Render() override;
	private:
		std::string mSourcePath;
		std::vector<MeshFragment*> mFragments;
		std::vector<Assets::Texture*> mTextures;
	};
}