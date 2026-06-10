#pragma once

#include <EngineClasses/Components/Mesh.h>
#include <EngineClasses/Components/APhysics.h>
#include "AObject.h"

namespace Refraction::Objects {
	class BasicObject : public AObject {
	public:
		BasicObject() {
			AddComponent<Components::Mesh>();
			AddComponent<Components::APhysics>();
		}
		~BasicObject();
	};
}
