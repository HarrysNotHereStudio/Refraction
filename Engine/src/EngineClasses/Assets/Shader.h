#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Matrices.h>

#include "Asset.h"

namespace EngineAssets {
	class Shader : public Asset {
	public:
		Shader(std::string sourcePath);

		void Activate() const;

		void SetUniformBool(const std::string& name, bool value) const;
		void SetUniformInt(const std::string& name, int value) const;
		void SetUniformFloat(const std::string& name, float value) const;
		void SetUniformVec2(const std::string& name, const glm::vec2& value) const;
		void SetUniformVec2(const std::string& name, const Refraction::Math::Vector2& value) const;
		void SetUniformVec2(const std::string& name, float x, float y) const;
		void SetUniformVec3(const std::string& name, const glm::vec3& value) const;
		void SetUniformVec3(const std::string& name, const Refraction::Math::Vector3& value) const;
		void SetUniformVec3(const std::string& name, float x, float y, float z) const;
		void SetUniformVec4(const std::string& name, const glm::vec4& value) const;
		void SetUniformVec4(const std::string& name, float x, float y, float z, float w) const;
		void SetUniformMat2(const std::string& name, const glm::mat2& matrix) const;
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void SetUniformMat3(const std::string& name, const Refraction::Math::Matrix3& matrix) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;
		void SetUniformMat4(const std::string& name, const Refraction::Math::Matrix4& matrix) const;

		std::string GetName() const { return mName; };
		std::string GetSourcePath() const { return mSourcePath; };
	protected:

	private:
		unsigned int mID = 0;
		std::string mSourcePath;
		std::string mName;

		void CheckLogErrors(GLuint shader, const std::string type);
	};
}
