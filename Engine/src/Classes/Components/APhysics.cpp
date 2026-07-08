#include <json.hpp>

#include <Classes/ClassSerialiser.h>

#include "APhysics.h"

namespace Refraction::Components {
	void APhysics::Tick(float delta) {
		if (mLinearVelocity.Magnitude() > 0) mParent->mTransform.Translate(mLinearVelocity * delta);
		if (mAngularVelocity.Magnitude() > 0) mParent->mTransform.Rotate(mAngularVelocity * delta);
	}

	std::string APhysics::Serialise() {
		using nlohmann::json;

		try {
			json serialised = json::parse(AComponent::Serialise());
			serialised["LinearVelocity"] = Utilities::ClassSerialiser::Serialise(mLinearVelocity);
			serialised["AngularVelocity"] = Utilities::ClassSerialiser::Serialise(mAngularVelocity);
			return serialised.dump();
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON: " + std::string(err.what()));
		}
	}

	void APhysics::Deserialise(std::string serialised) {
		using nlohmann::json;

		try {
			AComponent::Deserialise(serialised);
			json data = json::parse(serialised);
			mLinearVelocity = Utilities::ClassSerialiser::DeserialiseVector3(data.at("LinearVelocity"));
			mAngularVelocity = Utilities::ClassSerialiser::DeserialiseVector3(data.at("AngularVelocity"));
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON: " + std::string(err.what()));
		}
	}
}
