#include <Core/Utilities.h>

#include "Component.h"

Component::Component() {
	mUUID = Refraction::Utilities::GenerateUUID();
}

Component::~Component() {

}
