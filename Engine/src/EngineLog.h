#pragma once

#include <string>

#include <glm/glm.hpp>


class Log {
public:
	static std::string GenerateTimestamp();
	static void Info(std::string message);
	static void Warn(std::string message);
	static void Error(std::string message);
protected:
	static std::string ThreadName;
};

class RenderLog : public Log {
public:
	static std::string ToString(glm::vec3 vector);
	static std::string ToString(glm::quat quaternion);
	static std::string ToString(glm::mat4 matrix, bool pretty);
protected:
	static std::string ThreadName;
};

class RuntimeLog : public Log {
protected:
	static std::string ThreadName;
};

class PhysicsLog : public Log {
protected:
	static std::string ThreadName;
};
