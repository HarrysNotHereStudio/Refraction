

#include "Shader.h"

#include <Core/Log.h>
#include <Core/Utilities.h>

namespace RMath = Refraction::Math;
namespace RUtil = Refraction::Utilities;
using RMath::Vector2;
using RMath::Vector3;


EngineAssets::Shader::Shader(std::string sourcePath) {
	using std::vector, std::filesystem::directory_entry;
	mSourcePath = sourcePath;
	mName = sourcePath.substr(sourcePath.find_last_of("/") + 1);
	Refraction::RuntimeLog::Info("Creating shader " + mName);

	// Get all shader files (.vert and .frag) in the folder
	directory_entry vertShader = RUtil::GetFirstFileOfExtInFolder(mSourcePath, ".vert");
	directory_entry fragShader = RUtil::GetFirstFileOfExtInFolder(mSourcePath, ".frag");

	if (!(vertShader.exists() && fragShader.exists())) {
		Refraction::RuntimeLog::Warn("Skipping shader creation, missing source files");
		return;
	}

	std::string vertPath = vertShader.path().string();
	std::string fragPath = fragShader.path().string();

	std::string vertSource = RUtil::ReadFile(vertPath);
	std::string fragSource = RUtil::ReadFile(fragPath);

	const char* pVertSource = vertSource.data();
	const char* pFragSource = fragSource.data();

	unsigned int vert, frag;
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &pVertSource, NULL);
	glCompileShader(vert);
	CheckLogErrors(vert, "VERTEX");
	Refraction::RuntimeLog::Info("Compiled vertex shader");

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &pFragSource, NULL);
	glCompileShader(frag);
	CheckLogErrors(frag, "FRAGMENT");
	Refraction::RuntimeLog::Info("Compiled fragment shader");

	mID = glCreateProgram();
	glAttachShader(mID, vert);
	glAttachShader(mID, frag);
	glLinkProgram(mID);
	CheckLogErrors(mID, "PROGRAM");
	Refraction::RuntimeLog::Info("Linked shader program");

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void EngineAssets::Shader::Activate() const {
	glUseProgram(mID);
}

void EngineAssets::Shader::SetUniformBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value);
}

void EngineAssets::Shader::SetUniformInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}

void EngineAssets::Shader::SetUniformFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}

void EngineAssets::Shader::SetUniformVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void EngineAssets::Shader::SetUniformVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(mID, name.c_str()), x, y);
}

void EngineAssets::Shader::SetUniformVec2(const std::string& name, const Vector2& value) const {
	SetUniformVec2(name, value.x, value.y);
}

void EngineAssets::Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void EngineAssets::Shader::SetUniformVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(mID, name.c_str()), x, y, z);
}

void EngineAssets::Shader::SetUniformVec3(const std::string& name, const Vector3& value) const {
	SetUniformVec3(name, value.x, value.y, value.z);
}

void EngineAssets::Shader::SetUniformVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void EngineAssets::Shader::SetUniformVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(mID, name.c_str()), x, y, z, w);
}

void EngineAssets::Shader::SetUniformMat2(const std::string& name, const glm::mat2& matrix) const {
	glUniformMatrix2fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void EngineAssets::Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) const {
	glUniformMatrix3fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void EngineAssets::Shader::SetUniformMat3(const std::string& name, const Refraction::Math::Matrix3& matrix) const {
	SetUniformMat3(name, RUtil::NativeToGLMMat3(matrix));
}

void EngineAssets::Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void EngineAssets::Shader::SetUniformMat4(const std::string& name, const Refraction::Math::Matrix4& matrix) const {
	SetUniformMat4(name, RUtil::NativeToGLMMat4(matrix));
}

void EngineAssets::Shader::CheckLogErrors(GLuint shader, const std::string type) {
	GLint success;
	GLchar log[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, log);
			Refraction::RuntimeLog::Error("SHADER COMPILATION FAILED | " + type + "\n" + log + "\n--- COMPILE ERROR LOG END ---");
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, log);
			Refraction::RuntimeLog::Error("PROGRAM LINK FAILED | " + type + "\n" + log + "\n--- LINK ERROR LOG END ---");
		}
	}
}
