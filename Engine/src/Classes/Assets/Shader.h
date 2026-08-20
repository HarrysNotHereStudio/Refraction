#pragma once

#include <string>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Math/Vector.h>
#include <Math/Matrix.h>

#include "Asset.h"

namespace Refraction::Assets {
	struct ShaderMetadata : public AssetMetadata {
		size_t ProgramCount = 0;

		ShaderMetadata() = default;
		~ShaderMetadata() = default;

		std::string Serialise() override;
		void Deserialise(std::string data) override;
	};

	class Shader : public Asset {
	public:
		static Common::Ref<Shader> GetShaderByName(const std::string& name);
		static void LoadAllShaders();

		Shader() = default;
		virtual ~Shader();

		void Activate() const;

		void SetUniformBool(const std::string& name, bool value) const;
		void SetUniformInt(const std::string& name, int value) const;
		void SetUniformFloat(const std::string& name, float value) const;
		void SetUniformVec2(const std::string& name, const Math::Vector2& value) const;
		void SetUniformVec2(const std::string& name, float x, float y) const;
		void SetUniformVec3(const std::string& name, const Math::Vector3& value) const;
		void SetUniformVec3(const std::string& name, float x, float y, float z) const;
		void SetUniformVec4(const std::string& name, float x, float y, float z, float w) const;
		void SetUniformMat3(const std::string& name, Math::Matrix3 matrix) const;
		void SetUniformMat4(const std::string& name, Math::Matrix4 matrix) const;

		std::string GetName() const { return mName; };

		AssetMetadata& GetMetadata() override { return mMetadata; }
		void LoadAsset(const std::filesystem::path& source) override;
	private:
		ShaderMetadata mMetadata;
		unsigned int mID = 0;
		std::string mName;

		bool CheckLogErrors(GLuint shader, const std::string type);
	};
}
