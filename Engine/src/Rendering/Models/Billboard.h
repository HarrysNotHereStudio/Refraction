#pragma once

#include <Classes/Assets/Texture.h>
#include <Math/Transform.h>

class Billboard {
public:
	Refraction::Math::Transform* mTransform;
	bool mScreenSpaceScale = false;

	static void DrawAll();

	Billboard();
	~Billboard();

	void SetTexture(std::filesystem::path path);
protected:
	virtual void Draw();
private:
	Refraction::Assets::Texture* mTexture = nullptr;
};
