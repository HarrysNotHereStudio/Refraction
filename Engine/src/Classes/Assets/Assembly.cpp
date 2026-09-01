#include <json.hpp>

#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>
#include <Interface/AssetManager.h>

#include "Assembly.h"

using nlohmann::json;

namespace Refraction::Assets {
	Common::Shared<Objects::AObject> Assembly::Get() {
		auto meta = Engine::AssetManager::GetInstance()->FetchMetadata(GetUUID());
		return Deserialise(FileHandling::ReadFile(meta->AssetPath));
	}

	std::string Assembly::Serialise(Common::Shared<Objects::AObject> root) {
		return root->Serialise();
	}

	Common::Shared<Objects::AObject> Assembly::Deserialise(std::string tree) {
		return Utilities::ClassSerialiser::DeserialiseObject(tree);
	}
}

