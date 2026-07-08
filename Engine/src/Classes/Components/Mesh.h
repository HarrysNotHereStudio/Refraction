#pragma once

#include <vector>
#include <filesystem>

#include <Core/FileHandling.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Transform.h>
#include <Classes/Assets/Material.h>
#include <Classes/Assets/Texture.h>

#include "AComponent.h"

struct sVertex {
	Refraction::Math::Vector3 pos;
	Refraction::Math::Vector3 normal;
	Refraction::Math::Vector2 texCoord;
};

namespace Refraction::Components {
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

	class Mesh : public AComponent {
	public:
		Math::Transform mTransform;

		static int FrameMeshCount;
		static int FrameVertexCount;

		Mesh();

		void LoadModel(std::filesystem::path path);
		void Tick(float delta) override {};
		void Render() override;

		std::filesystem::path GetSource() { return mSourcePath; };

		std::string Serialise() override;
		void Deserialise(std::string serialised) override;
	private:
		std::filesystem::path mSourcePath;
		std::vector<MeshFragment*> mFragments;
		std::vector<Assets::Texture*> mTextures;
	};
}