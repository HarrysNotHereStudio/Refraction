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
		virtual ~AObject();

		// Returns a child component of a given type (if it exists)
		template<typename T>
		inline Common::Ref<T> GetComponent() {
			for (auto& comp : mComponents) {
				auto casted = dynamic_pointer_cast<T>(comp);
				if (casted) return casted;
			}
			return nullptr;
		}

		// Returns all children components
		ComponentList* GetComponents() { return &mComponents; }

		// Adds a new child component
		template<typename T>
		inline Common::Ref<T> AddComponent() {
			Common::Ref<T> newComp = Common::NewRef<T>();
			newComp->mParent = this;
			mComponents.push_back(newComp);
			return newComp;
		}


		// Returns a child object of a given type (if it exists)
		template<typename T>
		inline Common::Ref<T> GetFirstChild() {
			for (auto& obj : mChildren) {
				auto casted = dynamic_pointer_cast<T>(obj);
				if (casted) return casted;
			}
			return nullptr;
		}

		// Returns a child object with a given name (if it exists)
		Common::Ref<AObject> GetFirstChild(std::string name);

		// Returns all children objects
		inline ObjectList* GetChildren() { return &mChildren; }

		// Adds a given child object
		void AddChild(Common::Ref<AObject> child);

		// Removes itself from its parent
		void Remove();
		// Removes a given child object/component by UUID
		void RemoveChild(UUID target);

		// Creates a copy of this object and its descendants
		Common::Ref<AObject> Clone();
		
		// Returns the UUID of the object
		inline UUID GetUUID() const { return mUUID; }

		// Returns the world transform of this object (multiplied with ancestors)
		Math::Transform GetWorldTransform();

		// Returns a serialised copy of the object and its Components and children
		virtual std::string Serialise();
		// Loads data from the provided serialised object
		virtual void Deserialise(std::string serialised);
	protected:
		std::string mClassName;
		ComponentList mComponents;
		ObjectList mChildren;

	private:
		UUID mUUID;
	};
}
