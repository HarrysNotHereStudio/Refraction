#pragma once

#include <Classes/Components/Mesh.h>
#include <Classes/Components/APhysics.h>
#include "AObject.h"

namespace Refraction::Objects {
	class BasicObject : public AObject {
	public:
		BasicObject() {
			mInstanceName = "BasicObject";
			AddComponent<Components::Mesh>();
			AddComponent<Components::APhysics>();
		}
		~BasicObject();

		std::string Serialise() override;
		void Deserialise(std::string serialised) override;
	};
}
