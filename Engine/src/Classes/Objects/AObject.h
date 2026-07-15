#pragma once

#include <vector>
#include <string>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Math/Transform.h>

namespace Refraction::Components {
	class AComponent;
}

namespace Refraction::Objects {
	class AObject {
	public:
		typedef std::vector<Common::Ref<Components::AComponent>> ComponentList;
		typedef std::vector<Common::Ref<AObject>> ObjectList;

		Math::Transform mTransform;
		std::string mInstanceName = "Object";
		Objects::AObject* mParent = nullptr;

		// Returns an object under the given parent with the target UUID (or with a component with the target UUID)
		static AObject* GetInstanceWithUUID(UUID target, AObject* parent);

		AObject() {}
		AObject(const AObject& object);
		~AObject();

		// Returns a child component of a given type (if it exists)
		template<typename T>
		inline T* GetComponent() {
			for (auto& comp : mComponents) {
				T* casted = dynamic_cast<T*>(comp.get());
				if (casted) return casted;
			}
			return nullptr;
		}

		// Returns all children components
		ComponentList* GetComponents() { return &mComponents; }

		// Adds a new child component
		template<typename T>
		inline void AddComponent() {
			Common::Ref<Components::AComponent> newComp = Common::NewRef<T>();
			newComp->mParent = this;
			mComponents.push_back(newComp);
		}


		// Returns a child object of a given type (if it exists)
		template<typename T>
		inline T* GetFirstChild() {
			for (auto& obj : mChildren) {
				T* casted = dynamic_cast<T*>(obj.get());
				if (casted) return casted;
			}
			return nullptr;
		}

		// Returns a child object with a given name (if it exists)
		AObject* GetFirstChild(std::string name);

		// Returns all children objects
		inline ObjectList* GetChildren() { return &mChildren; }

		// Adds a given child object
		void AddChild(Common::Ref<AObject> child);

		// Removes a given child object/component by UUID
		void RemoveChild(UUID target);

		// Creates a copy of this object and its descendants
		Common::Ref<AObject> Clone();
		
		// Returns the UUID of the object
		inline UUID GetUUID() const { return mUUID; }

		// Returns a serialised copy of the object and its Components and children
		virtual std::string Serialise();
		// Loads data from the provided serialised object
		virtual void Deserialise(std::string serialised);
	protected:
		std::string mDisplayName;
		ComponentList mComponents;
		ObjectList mChildren;
	private:
		UUID mUUID;
	};
}
