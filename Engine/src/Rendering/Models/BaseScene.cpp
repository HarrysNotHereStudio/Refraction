#include "BaseScene.h"

using glm::vec3;

BaseScene::BaseScene(std::string resourcesPath) {
	Log::Info("Creating test model...");
	mNyen = new BaseModel(resourcesPath + "/models/nyen/nyen plush.obj");
	mModels.push_back(mNyen);

	InstancedModel* testModel2 = new InstancedModel(resourcesPath + "/models/survivalBackpack/backpack.obj");
	testModel2->mTransform->Translate(vec3(0.0f, 0.0f, 10.0f));
	testModel2->AddInstance(vec3(0.0f, 5.0f, 0.0f));
	testModel2->AddInstance(vec3(5.0f, 10.0f, 0.0f));
	testModel2->AddInstance(vec3(-3.0f, -5.0f, 0.0f));
	testModel2->mInstanceTransforms[1].Rotate(vec3(0.0f,glm::radians(30.0f),2.0f));
	mModels.push_back(testModel2);

	Log::Info("Instantiating lights...");
	for (int i = 0; i < 200; i++) {
		auto light = new PointLight();
		light->mTransform->Translate(vec3(Utilities::RandomI(20,-20), Utilities::RandomI(20, -20), Utilities::RandomI(20, -20)));
		light->mLightColor = vec3(1.0f, 1.0f, 1.0f);
		mLights.push_back(light);
	}
	auto light1 = new PointLight();
	light1->mTransform->Translate(vec3(3.0f, 1.0f, 3.0f));
	light1->mLightColor = vec3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light1);
	auto light2 = new PointLight();
	light2->mTransform->Translate(vec3(0.0f, -2.0f, 1.0f));
	light2->mLightColor = vec3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light2);
	auto light3 = new PointLight();
	light3->mTransform->Translate(vec3(2.0f, 4.0f, 6.0f));
	light3->mLightColor = vec3(1.0f, 0.2f, 0.2f);
	mLights.push_back(light3);
	auto light4 = new PointLight();
	light4->mTransform->Translate(vec3(0.0f, 6.0f, 9.0f));
	light4->mLightColor = vec3(0.2f, 0.2f, 1.0f);
	mLights.push_back(light4);
	auto light5 = new PointLight();
	light5->mTransform->Translate(vec3(4.0f, -2.0f, 12.0f));
	light5->mLightColor = vec3(1.0f, 1.0f, 1.0f);
	mLights.push_back(light5);
}

// TODO: Implement file loading
void BaseScene::LoadFromFile(std::string path) {
}

void BaseScene::Tick(float deltaTime) {
	auto yawDiff = 64.0f * deltaTime;

	// rotate nyen.
	mNyen->mTransform->orientation.y += yawDiff;
}