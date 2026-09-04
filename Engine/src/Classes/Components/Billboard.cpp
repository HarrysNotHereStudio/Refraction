#include <json.hpp>

#include <Classes/ClassSerialiser.h>
#include <Classes/Objects/Camera.h>
#include <Classes/Components/Mesh.h>
#include <Interface/AssetManager.h>

#include "Billboard.h"

namespace Refraction::Components {
	Billboard::Billboard() {
		mClassName = "BillboardComponent";
		mTransform.Scale(Math::Vector3(10));
	}

	void Billboard::Render() {
		if (mImage.expired()) return;

		Common::Ref<Assets::Shader> shaderWeak;
		Engine::AssetManager::Try([&](Common::Shared<Engine::AssetManager> assetManager) {
			shaderWeak = assetManager->GetAsset<Assets::Shader>("baseShader");
		});
		if (shaderWeak.expired()) return;

		using Math::Vector2, Math::Vector3;
		auto& camera = Objects::Camera::ActiveCamera;
		auto cameraUp = camera->mTransform.GetUpVector();
		auto cameraRight = camera->mTransform.GetRightVector();

		Engine::sVertex rectVertices[4] = {
			Engine::sVertex{ Vector3(-1,1,0), Vector3(0,0,1), Vector2(0,1) },
			Engine::sVertex{ Vector3(-1,-1,0), Vector3(0,0,1), Vector2(0,0) },
			Engine::sVertex{ Vector3(1,1,0), Vector3(0,0,1), Vector2(1,1) },
			Engine::sVertex{ Vector3(1,-1,0), Vector3(0,0,1), Vector2(1,0) }
		};

		for (size_t i = 0; i < 4; i++) {
			Engine::sVertex& vert = rectVertices[i];
			vert.pos = mTransform.GetWorldPosition() + cameraRight * vert.pos.x * mTransform.mScale.x + cameraUp * vert.pos.y * mTransform.mScale.y;
		}

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), &rectVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::sVertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::sVertex), (void*)offsetof(Engine::sVertex, texCoord));
		glEnableVertexAttribArray(2);

		if (shaderWeak.expired() || mImage.expired()) return;
		auto img = mImage.lock();
		if (img->mTexture.expired()) return;
		auto shader = shaderWeak.lock();
		auto tex = img->mTexture.lock();
		shader->Activate();
		tex->Activate(0);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	std::string Billboard::Serialise() {
		if (auto image = mImage.lock()) {
			return Utilities::ClassSerialiser::TryAppendJSON(AComponent::Serialise(), [&](nlohmann::json& json) {
				json["ImageUUID"] = image->GetUUID();
			});
		} else throw Common::RuntimeError("Could not serialise, failed to lock reference");
	}

	void Billboard::Deserialise(std::string serialised) {
		Engine::AssetManager::Try([&](Common::Shared<Engine::AssetManager> assetManager) {
			AComponent::Deserialise(serialised);
			Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
				if (json.contains("ImageUUID")) {
					mImage = assetManager->GetAsset<Assets::Image>(json.at("ImageUUID").get<uint64_t>());
				}
			});
		});
	}
}
