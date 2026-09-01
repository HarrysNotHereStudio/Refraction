#pragma once

#include "Asset.h"

#include <Classes/Objects/AObject.h>

namespace Refraction::Assets {
	class Assembly : public Asset {
	public:
		// Returns a new copy of the assembly
		Common::Shared<Objects::AObject> Get();

	private:
		// Serialises an object tree
		std::string Serialise(Common::Shared<Objects::AObject> root);
		// Deserialises into a tree of objects
		Common::Shared<Objects::AObject> Deserialise(std::string tree);
	};

}