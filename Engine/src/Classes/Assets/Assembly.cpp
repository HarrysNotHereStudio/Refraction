#include <json.hpp>

#include <Core/FileHandling.h>

#include <Classes/ClassSerialiser.h>

#include "Assembly.h"

using nlohmann::json;

namespace Refraction::Assets {
	Common::Ref<Objects::AObject> Assembly::Get() {
		return Deserialise(FileHandling::ReadFile(GetMetadata().AssetPath));
	}

	std::string Assembly::Serialise(Common::Ref<Objects::AObject> root) {
		return root->Serialise();
	}

	Common::Ref<Objects::AObject> Assembly::Deserialise(std::string tree) {
		return Utilities::ClassSerialiser::DeserialiseObject(tree);
	}
}

