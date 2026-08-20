#pragma once

#include "Asset.h"

#include <Classes/Objects/AObject.h>

namespace Refraction::Assets {
	class Assembly : public Asset {
	public:
		// Returns a new copy of the assembly
		Common::SRef<Objects::AObject> Get();

	private:
		// Serialises an object tree
		std::string Serialise(Common::SRef<Objects::AObject> root);
		// Deserialises into a tree of objects
		Common::SRef<Objects::AObject> Deserialise(std::string tree);
	};

}