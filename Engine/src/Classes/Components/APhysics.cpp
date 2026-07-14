#include <json.hpp>

#include <Classes/ClassSerialiser.h>

#include "APhysics.h"

namespace Refraction::Components {
	APhysics::APhysics() {
		mDisplayName = "APhysicsComponent";
	}

	void APhysics::Tick(float delta) {
		if (mLinearVelocity.Magnitude() > 0) mParent->mTransform.Translate(mLinearVelocity * delta);
		if (mAngularVelocity.Magnitude() > 0) mParent->mTransform.Rotate(mAngularVelocity * delta);
	}

	std::string APhysics::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AComponent::Serialise(), [&](nlohmann::json& json) {
			json["LinearVelocity"] = Utilities::ClassSerialiser::Serialise(mLinearVelocity);
			json["AngularVelocity"] = Utilities::ClassSerialiser::Serialise(mAngularVelocity);
		});
	}

	void APhysics::Deserialise(std::string serialised) {
		AComponent::Deserialise(serialised);
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mLinearVelocity = Utilities::ClassSerialiser::DeserialiseVector3(json.at("LinearVelocity"));
			mAngularVelocity = Utilities::ClassSerialiser::DeserialiseVector3(json.at("AngularVelocity"));
		});
	}
}
