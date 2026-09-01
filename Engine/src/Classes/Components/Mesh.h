#pragma once

#include <vector>
#include <filesystem>

#include <Core/FileHandling.h>
#include <Math/Vector.h>
#include <Math/Transform.h>
#include <Classes/Assets/Material.h>
#include <Classes/Assets/Model.h>
#include <Platform/AMeshFragment.h>

#include "AComponent.h"

namespace Refraction::Components {
	class Mesh : public AComponent {
	public:
		Math::Transform mTransform;
		std::vector<Common::Shared<Assets::Material>> mMaterials;

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
		Common::Shared<Assets::Model> mModel;
	};
}