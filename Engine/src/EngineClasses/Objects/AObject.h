#pragma once

#include <vector>
#include <string>

#include <Core/Constants.h>
#include <Core/Utilities.h>
#include <Core/Log.h>
#include <Math/Transform.h>

namespace Refraction::Components {
	class AComponent;
}

namespace Refraction::Objects {
	class AObject {
	public:
		Math::Transform mTransform;
		std::string mInstanceName;

		AObject() {}

		// Returns a child component of a given type (if it exists)
		template<typename T>
		T* GetComponent() {
			for (auto& comp : mComponents) {
				T* casted = dynamic_cast<T*>(comp.get());
				if (casted) return casted;
			}
			return nullptr;
		}
		std::vector<Common::Ref<Components::AComponent>>* GetComponents() { return &mComponents; }
		template<typename T>
		void AddComponent() {
			Common::Ref<Components::AComponent> newComp = Common::NewRef<T>();
			newComp->mParent = this;
			mComponents.push_back(newComp);
		}
	protected:
		std::string mDisplayName;
		std::vector<Common::Ref<Components::AComponent>> mComponents;
	};
}
