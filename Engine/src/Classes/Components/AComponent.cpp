#include <json.hpp>

#include <Core/Utilities.h>

#include "AComponent.h"

namespace Refraction::Components {
	AComponent::AComponent() {
		mUUID = UUID();
	}

	AComponent::~AComponent() {
		mUUID.Reset();
	}

	std::string AComponent::Serialise() {
		using nlohmann::json;
		json serialised;
		serialised["UUID"] = mUUID.Serialise();
		serialised["TypeName"] = typeid(*this).name();
		Log::SInfo("Serialising as " + std::string(typeid(*this).name()));
		serialised["DisplayName"] = mDisplayName;
		serialised["ParentUUID"] = mParent ? mParent->GetUUID().Serialise() : UUID::Null().Serialise();

		return serialised.dump();
	}

	void AComponent::Deserialise(std::string serialised) {
		using nlohmann::json;

		try {
			json data = json::parse(serialised);
			mDisplayName = data.at("DisplayName").get<std::string>();
			mUUID = UUID::Deserialise(data.at("UUID"));
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON serialised Component data");
		}
	}
}
