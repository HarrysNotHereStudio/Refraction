#pragma once

#include <json.hpp>

#include <string>
#include <functional>

#include <Core/Common.h>
#include <Classes/Components/AComponent.h>
#include <Classes/Objects/AObject.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Math/Orientation.h>
#include <Math/Rect.h>
#include <Math/Frustum.h>
#include <Math/Transform.h>

namespace Refraction::Utilities {
	class ClassSerialiser {
	public:
		// Tries to parse a json dump and outputs any error
		static void TryParseJSON(std::string dump, std::function<void(nlohmann::json&)> fn);
		// Tries to append to a parsed json dump and outputs any error
		static std::string TryAppendJSON(std::string dump, std::function<void(nlohmann::json&)> fn);

		static std::string Serialise(Common::SRef<Objects::AObject> object);
		static std::string Serialise(Common::SRef<Components::AComponent> comp);
		static Common::SRef<Objects::AObject> DeserialiseObject(std::string serialisedData);
		static Common::SRef<Components::AComponent> DeserialiseComponent(std::string serialisedData);

		template<typename ObjectType>
		static Common::SRef<ObjectType> DeserialiseObject(std::string serialisedData) {
			auto obj = DeserialiseObject(serialisedData);
			return dynamic_pointer_cast<ObjectType>(obj);
		}
		template<typename ComponentType>
		static Common::SRef<ComponentType> DeserialiseComponent(std::string serialisedData) {
			auto comp = DeserialiseComponent(serialisedData);
			return dynamic_pointer_cast<ComponentType>(comp);
		}


		static std::string Serialise(Math::Vector2 vec);
		static std::string Serialise(Math::Vector3 vec);
		static std::string Serialise(Math::Vector4 vec);
		static std::string Serialise(Math::Quaternion quat);
		static std::string Serialise(Math::Orientation orient);
		static std::string Serialise(Math::Rect rect);
		static std::string Serialise(Math::Frustum frustum);
		static std::string Serialise(Math::Transform transform);
		static Math::Vector2 DeserialiseVector2(std::string serialisedData);
		static Math::Vector3 DeserialiseVector3(std::string serialisedData);
		static Math::Vector4 DeserialiseVector4(std::string serialisedData);
		static Math::Quaternion DeserialiseQuaternion(std::string serialisedData);
		static Math::Orientation DeserialiseOrientation(std::string serialisedData);
		static Math::Rect DeserialiseRect(std::string serialisedData);
		static Math::Frustum DeserialiseFrustum(std::string serialisedData);
		static Math::Transform DeserialiseTransform(std::string serialisedData);
	};
}


