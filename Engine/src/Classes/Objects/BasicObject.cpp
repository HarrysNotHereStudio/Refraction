#include "BasicObject.h"

namespace Refraction::Objects {
	BasicObject::~BasicObject() {}
	nlohmann::json BasicObject::Serialise() {
		return AObject::Serialise();
	}
	void BasicObject::Deserialise(std::string serialised) {
		AObject::Deserialise(serialised);
	}
}
