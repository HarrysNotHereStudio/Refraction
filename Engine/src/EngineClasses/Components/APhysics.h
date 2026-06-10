#pragma once

#include <Math/Vector3.h>
#include "AComponent.h"

namespace Refraction::Components {
	class APhysics : public AComponent {
	public:
		Math::Vector3 mLinearVelocity;
		Math::Vector3 mAngularVelocity;

		void Tick(float delta) override;
	private:

	};

}