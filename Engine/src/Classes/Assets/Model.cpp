#include <Classes/ClassSerialiser.h>

#include "Model.h"

namespace Refraction::Assets {
	std::string ModelMetadata::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AssetMetadata::Serialise(), [&](nlohmann::json& json) {
			json["VertexCount"] = VertexCount;
			json["PolyCount"] = PolyCount;
		});
	}

	void ModelMetadata::Deserialise(std::string data) {
		AssetMetadata::Deserialise(data);
		Utilities::ClassSerialiser::TryParseJSON(data, [&](nlohmann::json& json) {
			VertexCount = json.at("VertexCount").get<int>();
			PolyCount = json.at("PolyCount").get<int>();
		});
	}

	void Model::InternalLoadAsset(Common::Shared<AssetMetadata> metadata) {
		auto meta = Common::AsA<ModelMetadata>(metadata);
		if (!meta) {
			Log::SError("Metadata cast failed");
			return;
		}

		// SOLUTION: we load it HERE because we save by only having ONE copy of the data (Mesh objects just hold references to ts, if they need to copy it then so be it)
		// TODO: Load mesh data here
	}
}

