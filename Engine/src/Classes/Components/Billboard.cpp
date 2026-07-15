#include <json.hpp>

#include <Classes/ClassSerialiser.h>
#include <Classes/Objects/Camera.h>
#include <Classes/Components/Mesh.h>

#include "Billboard.h"

namespace Refraction::Components {
	Billboard::Billboard() {
		mClassName = "BillboardComponent";
		mTransform.Scale(Math::Vector3(10));
	}

	void Billboard::Render() {
		if (!mTexture) return;
		using Math::Vector2, Math::Vector3;
		auto& camera = Objects::Camera::ActiveCamera;
		auto cameraUp = camera->mTransform.GetUpVector();
		auto cameraRight = camera->mTransform.GetRightVector();

		sVertex rectVertices[4] = {
			sVertex{ Vector3(-1,1,0), Vector3(0,0,1), Vector2(0,1) },
			sVertex{ Vector3(-1,-1,0), Vector3(0,0,1), Vector2(0,0) },
			sVertex{ Vector3(1,1,0), Vector3(0,0,1), Vector2(1,1) },
			sVertex{ Vector3(1,-1,0), Vector3(0,0,1), Vector2(1,0) }
		};

		for (size_t i = 0; i < 4; i++) {
			sVertex& vert = rectVertices[i];
			vert.pos = mTransform.GetWorldPosition() + cameraRight * vert.pos.x * mTransform.mScale.x + cameraUp * vert.pos.y * mTransform.mScale.y;
		}

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), &rectVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, texCoord));
		glEnableVertexAttribArray(2);

		Assets::Shader::GetShaderByName("baseShader")->Activate();
		mTexture->Activate(0);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	std::string Billboard::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AComponent::Serialise(), [&](nlohmann::json& json) {
			json["TextureUUID"] = mTexture->GetMetadata().AssetUUID.Serialise();
		});
	}

	void Billboard::Deserialise(std::string serialised) {
		AComponent::Deserialise(serialised);
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			if (json.contains("TextureUUID")) {
				mTexture = Assets::Asset::GetAsset<Assets::Texture>(json.at("TextureUUID").get<uint64_t>());
			}
		});
	}
}
