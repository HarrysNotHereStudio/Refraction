#include <json.hpp>

#include <Classes/ClassHeaders.h>

#include "ClassSerialiser.h"

using nlohmann::json;

namespace Refraction::Utilities {
	void ClassSerialiser::TryParseJSON(std::string dump, std::function<void(json&)> fn) {
		try {
			json data = json::parse(dump);
			fn(data);
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}

	std::string ClassSerialiser::TryAppendJSON(std::string dump, std::function<void(json&)> fn) {
		std::string result;
		TryParseJSON(dump, [&](json& data) {
			fn(data);
			result = data.dump();
		});
		return result;
	}

	std::string ClassSerialiser::Serialise(Common::Ref<Objects::AObject> object) {
		return object->Serialise();
	}

	std::string ClassSerialiser::Serialise(Common::Ref<Components::AComponent> component) {
		return component->Serialise();
	}

	Common::Ref<Objects::AObject> ClassSerialiser::DeserialiseObject(std::string serialisedData) {
		try {
			Common::Ref<Objects::AObject> deserialised;
			json data = json::parse(serialisedData);

			auto className = data.at("TypeName").get<std::string>();
			if (className == typeid(Objects::AObject).name()) {
				deserialised = Common::NewRef<Objects::AObject>();
			} else if (className == typeid(Objects::BasicObject).name()) {
				deserialised = Common::NewRef<Objects::BasicObject>();
			} else if (className == typeid(Objects::SceneRoot).name()) {
				deserialised = Common::NewRef<Objects::SceneRoot>();
			} else if (className == typeid(Objects::Camera).name()) {
				deserialised = Common::NewRef<Objects::Camera>();
			}
			Log::SInfo("Deserialising object of type " + className);
			deserialised->Deserialise(serialisedData);
			return deserialised;
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}

	Common::Ref<Components::AComponent> ClassSerialiser::DeserialiseComponent(std::string serialisedData) {
		try {
			Common::Ref<Components::AComponent> deserialised;
			json data = json::parse(serialisedData);

			auto className = data.at("TypeName").get<std::string>();
			if (className == typeid(Components::AComponent).name()) {
				deserialised = Common::NewRef<Components::AComponent>();
			} else if (className == typeid(Components::APhysics).name()) {
				deserialised = Common::NewRef<Components::APhysics>();
			} else if (className == typeid(Components::Mesh).name()) {
				deserialised = Common::NewRef<Components::Mesh>();
			}
			Log::SInfo("Deserialising component of type " + className);
			deserialised->Deserialise(serialisedData);
			return deserialised;
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}

	std::string ClassSerialiser::Serialise(Math::Vector2 vec) {
		json result;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Vector3 vec) {
		json result;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		result["Z"] = vec.z;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Vector4 vec) {
		json result;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		result["Z"] = vec.z;
		result["W"] = vec.w;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Quaternion quat) {
		json result;
		result["X"] = quat.x;
		result["Y"] = quat.y;
		result["Z"] = quat.z;
		result["W"] = quat.w;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Rect rect) {
		json result;
		result["X"] = rect.x;
		result["Y"] = rect.y;
		result["W"] = rect.w;
		result["H"] = rect.h;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Frustum frustum) {
		json result;
		result["FovY"] = frustum.fovY;
		result["W"] = frustum.w;
		result["H"] = frustum.h;
		result["ZNear"] = frustum.zNear;
		result["ZFar"] = frustum.zFar;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Transform transform) {
		json result;
		result["SpatialPosition"]["GridIndex"] = Serialise(transform.mSpatialPosition.GridIndex);
		result["SpatialPosition"]["CellPosition"] = Serialise(transform.mSpatialPosition.CellPosition);
		result["Orientation"] = Serialise(transform.mOrientation);
		result["Scale"] = Serialise(transform.mScale);
		return result.dump();
	}
	Math::Vector2 ClassSerialiser::DeserialiseVector2(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Vector2(data.at("X").get<float>(), data.at("Y").get<float>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Vector3 ClassSerialiser::DeserialiseVector3(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Vector3(data.at("X").get<float>(), data.at("Y").get<float>(), data.at("Z").get<float>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Vector4 ClassSerialiser::DeserialiseVector4(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Vector4(data.at("X").get<float>(), data.at("Y").get<float>(), data.at("Z").get<float>(), data.at("W").get<float>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Quaternion ClassSerialiser::DeserialiseQuaternion(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Quaternion(data.at("X").get<float>(), data.at("Y").get<float>(), data.at("Z").get<float>(), data.at("W").get<float>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Rect ClassSerialiser::DeserialiseRect(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Rect(data.at("X").get<int>(), data.at("Y").get<int>(), data.at("W").get<int>(), data.at("H").get<int>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Frustum ClassSerialiser::DeserialiseFrustum(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Frustum(Math::Vector2(data.at("W").get<float>(), data.at("H").get<float>()), data.at("FovY").get<float>(), data.at("ZNear").get<float>(), data.at("ZFar").get<float>());
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
	Math::Transform ClassSerialiser::DeserialiseTransform(std::string serialisedData) {
		try {
			Math::Transform result;
			json data = json::parse(serialisedData);
			result.mSpatialPosition.GridIndex = DeserialiseVector3(data.at("SpatialPosition").at("GridIndex"));
			result.mSpatialPosition.CellPosition = DeserialiseVector3(data.at("SpatialPosition").at("CellPosition"));
			result.mOrientation = DeserialiseQuaternion(data.at("Orientation"));
			result.mScale = DeserialiseVector3(data.at("Scale"));
			return result;
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
}
