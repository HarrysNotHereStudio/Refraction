#include <EngineUtilities.h>

#include "Component.h"

Component::Component() {
	mUUID = Utilities::GenerateUUID();
}

Component::~Component() {

}
