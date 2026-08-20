#pragma once

#include <string>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Classes/Objects/AObject.h>

namespace Refraction::Components {
	class AComponent {
	public:
		Objects::AObject* mParent = nullptr;
		bool mRequired = false; // Determines whether this component is required by its parent object

		AComponent();
		virtual ~AComponent();

		virtual void PreTick() {};
		virtual void Tick(float delta) {};
		virtual void PostTick() {};
		virtual void PreRender() {};
		virtual void Render() {};
		virtual void PostRender() {};

		inline UUID GetUUID() const { return mUUID; }
		inline std::string GetDisplayName() const { return mClassName; };

		// Returns a serialised copy of the component
		virtual std::string Serialise();
		// Loads data from the provided serialised component
		virtual void Deserialise(std::string serialised);

	protected:
		std::string mClassName = "BaseComponent";
	private:
		UUID mUUID;
	};
}