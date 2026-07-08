#pragma once

#include "Asset.h"

#include <Classes/Objects/AObject.h>

namespace Refraction::Assets {
	class Assembly : public Asset {
	public:
		// Returns a new copy of the assembly
		Common::Ref<Objects::AObject> Get();

	private:
		// Serialises an object tree
		std::string Serialise(Common::Ref<Objects::AObject> root);
		// Deserialises into a tree of objects
		Common::Ref<Objects::AObject> Deserialise(std::string tree);
	};

}