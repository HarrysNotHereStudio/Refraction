#include <json.hpp>

#include <Core/FileHandling.h>

#include <Classes/ClassSerialiser.h>

#include "Assembly.h"

using nlohmann::json;

namespace Refraction::Assets {
	Common::SRef<Objects::AObject> Assembly::Get() {
		return Deserialise(FileHandling::ReadFile(GetMetadata().AssetPath));
	}

	std::string Assembly::Serialise(Common::SRef<Objects::AObject> root) {
		return root->Serialise();
	}

	Common::SRef<Objects::AObject> Assembly::Deserialise(std::string tree) {
		return Utilities::ClassSerialiser::DeserialiseObject(tree);
	}
}

