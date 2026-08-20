#pragma once

#include <Math/Vector.h>
#include "AComponent.h"

namespace Refraction::Components {
	class APhysics : public AComponent {
	public:
		Math::Vector3 mLinearVelocity;
		Math::Vector3 mAngularVelocity;

		APhysics();

		void Tick(float delta) override;

		std::string Serialise() override;
		void Deserialise(std::string serialised) override;
	};

}