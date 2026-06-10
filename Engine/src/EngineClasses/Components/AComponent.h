#pragma once

#include <string>

#include <Core/Constants.h>
#include <Core/Utilities.h>
#include <Core/Log.h>
#include <EngineClasses/Objects/AObject.h>

namespace Refraction::Components {
	class AComponent {
	public:
		Objects::AObject* mParent = nullptr;

		AComponent();
		~AComponent();

		virtual void PreTick() {};
		virtual void Tick(float delta) = 0;
		virtual void PostTick() {};
		virtual void PreRender() {};
		virtual void Render() {};
		virtual void PostRender() {};

		std::string GetUUID() const { return mUUID; }

	protected:
		std::string mDisplayName = "BaseComponent";
	private:
		std::string mUUID;
	};
}