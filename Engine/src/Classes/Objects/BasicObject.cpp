#include "BasicObject.h"

namespace Refraction::Objects {
	BasicObject::~BasicObject() {}
	std::string BasicObject::Serialise() {
		return AObject::Serialise();
	}
	void BasicObject::Deserialise(std::string serialised) {
		AObject::Deserialise(serialised);
	}
}
