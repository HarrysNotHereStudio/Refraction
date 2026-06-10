#include <vector>

#include <Math/Vector3.h>
#include <EngineClasses/Components/Mesh.h>
#include "BaseCamera.h"

#include "Billboard.h"

namespace RMath = Refraction::Math;
using RMath::Transform;
using RMath::Vector2;
using RMath::Vector3;

std::vector<Billboard*> ActiveBillboards = {};

void Billboard::DrawAll() {
	for (auto& billboard : ActiveBillboards) {
		billboard->Draw();
	}
}

Billboard::Billboard() {
	mTransform = new Transform();
	mTransform->mScale = Vector3(10);
}

Billboard::~Billboard() {
	delete mTransform;
	mTransform = nullptr;
	mTexture = nullptr;
}


void Billboard::SetTexture(std::string path) {
	mTexture = Refraction::Assets::Texture::GetTexture(path, REFRACT_TEXTURE_TYPE_DIFFUSE);
}

void Billboard::Draw() {
	auto& camera = BaseCamera::ActiveCamera;
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
		vert.pos = mTransform->GetWorldPosition() + cameraRight * vert.pos.x * mTransform->mScale.x + cameraUp * vert.pos.y * mTransform->mScale.y;
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

	//ShaderManager::GetShaderByName("gbufferShader")->Activate();
	mTexture->Activate(0);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}