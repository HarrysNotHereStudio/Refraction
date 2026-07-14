#include <json.hpp>

#include <Core/Utilities.h>
#include <Classes/ClassSerialiser.h>

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
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mDisplayName = json.at("DisplayName").get<std::string>();
			mUUID = UUID::Deserialise(json.at("UUID"));
		});
	}
}
