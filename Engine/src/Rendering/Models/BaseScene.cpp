#include <Core/Constants.h>
#include <Math/Vector3.h>
#include <EngineClasses/Components/Mesh.h>
#include <EngineClasses/Components/APhysics.h>
#include <EngineClasses/Objects/BasicObject.h>

#include "BaseScene.h"

using namespace Refraction;
using Math::Vector3;


BaseScene::BaseScene() {
	Log::Info("Creating test model...");
	auto nyen = Common::NewRef<Objects::BasicObject>();
	nyen->GetComponent<Components::Mesh>()->LoadModel(Constants::GetResourcePath() + "/models/nyen/nyen plush.obj");
	nyen->GetComponent<Components::APhysics>()->mAngularVelocity = Vector3(0, 64, 0);
	mObjects.push_back(nyen);
	mNyen = nyen;

	auto backpack = Common::NewRef<Objects::BasicObject>();
	backpack->GetComponent<Components::Mesh>()->LoadModel(Constants::GetResourcePath() + "/models/survivalBackpack/backpack.obj");
	backpack->mTransform = Math::Transform::FromLookAt(Vector3(0, 4, 10), Vector3::Zero());
	mObjects.push_back(backpack);

	Refraction::Log::Info("Instantiating lights...");
	for (int i = 0; i < 27; i++) {
		auto light = new PointLight();
		light->mTransform->Translate(Vector3(Utilities::RandomI(20,-20), Utilities::RandomI(20, -20), Utilities::RandomI(20, -20)));
		light->mLightColor = Vector3(1.0f, 1.0f, 1.0f);
		mLights.push_back(light);
	}
	auto light1 = new PointLight();
	light1->mTransform->Translate(Vector3(3.0f, 1.0f, 3.0f));
	light1->mLightColor = Vector3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light1);
	auto light2 = new PointLight();
	light2->mTransform->Translate(Vector3(0.0f, -2.0f, 1.0f));
	light2->mLightColor = Vector3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light2);
	auto light3 = new PointLight();
	light3->mTransform->Translate(Vector3(2.0f, 4.0f, 6.0f));
	light3->mLightColor = Vector3(1.0f, 0.2f, 0.2f);
	mLights.push_back(light3);
	auto light4 = new PointLight();
	light4->mTransform->Translate(Vector3(0.0f, 6.0f, 9.0f));
	light4->mLightColor = Vector3(0.2f, 0.2f, 1.0f);
	mLights.push_back(light4);
	auto light5 = new PointLight();
	light5->mTransform->Translate(Vector3(4.0f, -2.0f, 12.0f));
	light5->mLightColor = Vector3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light5);
}

// TODO: Implement file loading
void BaseScene::LoadFromFile(std::string path) {
}

void BaseScene::Tick(float deltaTime) {
	for (auto& object : mObjects) {
		auto comps = object->GetComponents();
		for (size_t i = 0; i < comps->size(); i++) {
			(*comps)[i]->Tick(deltaTime);
		}
	}
}

void BaseScene::Render() {
	for (auto& object : mObjects) {
		auto comps = object->GetComponents();
		for (size_t i = 0; i < comps->size(); i++) {
			(*comps)[i]->Render();
		}
	}
}
