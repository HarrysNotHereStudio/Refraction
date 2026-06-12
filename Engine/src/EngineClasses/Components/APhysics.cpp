#include "APhysics.h"

namespace Refraction::Components {
	void APhysics::Tick(float delta) {
		if (mLinearVelocity.Magnitude() > 0) mParent->mTransform.Translate(mLinearVelocity * delta);
		if (mAngularVelocity.Magnitude() > 0) mParent->mTransform.Rotate(mAngularVelocity * delta);
	}
}
