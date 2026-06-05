#pragma once

#include <EngineClasses/Assets/Texture.h>
#include <Math/Transform.h>

class Billboard {
public:
	Refraction::Math::Transform* mTransform;
	bool mScreenSpaceScale = false;

	static void DrawAll();

	Billboard();
	~Billboard();

	void SetTexture(std::string path);
protected:
	virtual void Draw();
private:
	EngineAssets::Texture* mTexture = nullptr;
};
