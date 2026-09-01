#include <json.hpp>

#include <Classes/ClassHeaders.h>
#include <Interface/AssetManager.h>

#include "ClassSerialiser.h"

using nlohmann::json;

namespace Refraction::Utilities {
	void ClassSerialiser::TryParseJSON(std::string dump, std::function<void(json&)> fn) {
		try {
			json data = json::parse(dump);
			fn(data);
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		} catch (const std::out_of_range& err) {
			throw std::runtime_error("JSON Index Error: " + std::string(err.what()));
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

	std::string ClassSerialiser::Serialise(Common::Shared<Assets::Asset> asset) {
		auto meta = Engine::AssetManager::GetInstance()->FetchMetadata(asset->GetUUID());
		return meta->Serialise();

	}
	std::string ClassSerialiser::Serialise(Common::Shared<Objects::AObject> object) {
		return object->Serialise();
	}

	std::string ClassSerialiser::Serialise(Common::Shared<Components::AComponent> component) {
		return component->Serialise();
	}

	Common::Shared<Assets::Asset> ClassSerialiser::DeserialiseAsset(Common::Shared<Assets::AssetMetadata> metadata) {
		Common::Shared<Assets::Asset> deserialised;
		if (metadata->AssetType == typeid(Assets::Asset).name()) {
			deserialised = Common::NewShared<Assets::Asset>();
		} else if (metadata->AssetType == typeid(Assets::Image).name()) {
			deserialised = Common::NewShared<Assets::Image>();
		} else if (metadata->AssetType == typeid(Assets::Shader).name()) {
			deserialised = Common::NewShared<Assets::Shader>();
		} else if (metadata->AssetType == typeid(Assets::Material).name()) {
			deserialised = Common::NewShared<Assets::Material>();
		} else if (metadata->AssetType == typeid(Assets::Assembly).name()) {
			deserialised = Common::NewShared<Assets::Assembly>();
		}

		Log::SInfo("Loading asset of type " + metadata->AssetType);
		deserialised->LoadAsset(metadata->AssetUUID);
		return deserialised;
	}

	Common::Shared<Objects::AObject> ClassSerialiser::DeserialiseObject(std::string serialisedData) {
		Common::Shared<Objects::AObject> deserialised;
		TryParseJSON(serialisedData, [&](nlohmann::json& data) {
			auto className = data.at("TypeName").get<std::string>();
			if (className == typeid(Objects::AObject).name()) {
				deserialised = Common::NewShared<Objects::AObject>();
			} else if (className == typeid(Objects::BasicObject).name()) {
				deserialised = Common::NewShared<Objects::BasicObject>();
			} else if (className == typeid(Objects::SceneRoot).name()) {
				deserialised = Common::NewShared<Objects::SceneRoot>();
			} else if (className == typeid(Objects::Camera).name()) {
				deserialised = Common::NewShared<Objects::Camera>();
			}

			Log::SInfo("Deserialising object of type " + className);
			deserialised->Deserialise(serialisedData);
		});
		return deserialised;
	}

	Common::Shared<Components::AComponent> ClassSerialiser::DeserialiseComponent(std::string serialisedData) {
		Common::Shared<Components::AComponent> deserialised;
		TryParseJSON(serialisedData, [&](nlohmann::json& data) {

			auto className = data.at("TypeName").get<std::string>();
			if (className == typeid(Components::AComponent).name()) {
				deserialised = Common::NewShared<Components::AComponent>();
			} else if (className == typeid(Components::APhysics).name()) {
				deserialised = Common::NewShared<Components::APhysics>();
			} else if (className == typeid(Components::Mesh).name()) {
				deserialised = Common::NewShared<Components::Mesh>();
			}

			Log::SInfo("Deserialising component of type " + className);
			deserialised->Deserialise(serialisedData);
		});
		return deserialised;
	}

	std::string ClassSerialiser::Serialise(Math::Vector2 vec) {
		json result;
		if (vec.x != vec.x) vec.x = 0;
		if (vec.y != vec.y) vec.y = 0;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Vector3 vec) {
		json result;
		if (vec.x != vec.x) vec.x = 0;
		if (vec.y != vec.y) vec.y = 0;
		if (vec.z != vec.z) vec.z = 0;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		result["Z"] = vec.z;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Vector4 vec) {
		json result;
		if (vec.x != vec.x) vec.x = 0;
		if (vec.y != vec.y) vec.y = 0;
		if (vec.z != vec.z) vec.z = 0;
		if (vec.w != vec.w) vec.w = 0;
		result["X"] = vec.x;
		result["Y"] = vec.y;
		result["Z"] = vec.z;
		result["W"] = vec.w;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Quaternion quat) {
		json result;
		if (quat.x != quat.x) quat.x = 0;
		if (quat.y != quat.y) quat.y = 0;
		if (quat.z != quat.z) quat.z = 0;
		if (quat.w != quat.w) quat.w = 0;
		result["X"] = quat.x;
		result["Y"] = quat.y;
		result["Z"] = quat.z;
		result["W"] = quat.w;
		return result.dump();
	}
	std::string ClassSerialiser::Serialise(Math::Orientation orient) {
		json result;
		if (orient.mPitch != orient.mPitch) orient.mPitch = 0;
		if (orient.mYaw != orient.mYaw) orient.mYaw = 0;
		if (orient.mRoll != orient.mRoll) orient.mRoll = 0;
		result["Pitch"] = orient.mPitch;
		result["Yaw"] = orient.mYaw;
		result["Roll"] = orient.mRoll;
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
	Math::Orientation ClassSerialiser::DeserialiseOrientation(std::string serialisedData) {
		try {
			json data = json::parse(serialisedData);
			return Math::Vector3(data.at("Pitch").get<float>(), data.at("Yaw").get<float>(), data.at("Roll").get<float>());
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
			result.mOrientation = DeserialiseOrientation(data.at("Orientation"));
			result.mScale = DeserialiseVector3(data.at("Scale"));
			return result;
		} catch (const json::parse_error& err) {
			throw std::runtime_error("JSON Parse Error: " + std::string(err.what()));
		}
	}
}
