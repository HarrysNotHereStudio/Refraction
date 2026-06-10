#include "APhysics.h"

namespace Refraction::Components {
	void APhysics::Tick(float delta) {
		mParent->mTransform.Translate(mLinearVelocity * delta);
		mParent->mTransform.Rotate(mAngularVelocity * delta);
	}
}
