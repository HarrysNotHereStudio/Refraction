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
		//Log::SInfo("Serialising as " + std::string(typeid(*this).name()));
		json serialised;
		serialised["UUID"] = mUUID.Serialise();
		serialised["TypeName"] = typeid(*this).name();
		serialised["ClassName"] = mClassName;
		serialised["ParentUUID"] = mParent ? mParent->GetUUID().Serialise() : UUID::Null().Serialise();
		serialised["Required"] = mRequired;

		return serialised.dump();
	}

	void AComponent::Deserialise(std::string serialised) {
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mClassName = json.at("ClassName").get<std::string>();
			mUUID = UUID::Deserialise(json.at("UUID"));
			mRequired = json.at("Required").get<bool>();
		});
	}
}
