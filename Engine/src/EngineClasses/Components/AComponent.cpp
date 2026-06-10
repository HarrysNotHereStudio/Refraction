#include <Core/Utilities.h>

#include "AComponent.h"

namespace Refraction::Components {
	AComponent::AComponent() {
		mUUID = Refraction::Utilities::GenerateUUID();
	}

	AComponent::~AComponent() {

	}
}
