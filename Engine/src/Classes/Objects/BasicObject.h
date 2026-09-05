#pragma once

#include <Classes/Components/Mesh.h>
#include <Classes/Components/APhysics.h>
#include "AObject.h"

namespace Refraction::Objects {
	class BasicObject : public AObject {
	public:
		BasicObject() {
			mInstanceName = "BasicObject";
			AddComponent<Components::Mesh>()->mRequired = true;
			AddComponent<Components::APhysics>()->mRequired = true;
		}
		virtual ~BasicObject();

		nlohmann::json Serialise() override;
		void Deserialise(std::string serialised) override;
	};
}
