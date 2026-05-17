#pragma once

#include <EngineClasses/Assets/Texture.h>
#include "Transform.h"

class Billboard {
public:
	Transform* mTransform;
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
