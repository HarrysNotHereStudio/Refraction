#pragma once

#include "AObject.h"

namespace Refraction::Objects {
	class SceneRoot : public AObject {
	public:
		SceneRoot() {
			mInstanceName = "SceneRoot";
		}

		void TickScene();
		void RenderScene();

		std::string Serialise() override;
		void Deserialise(std::string serialised) override;
	private:
		void PreTick(AObject* object);
		void Tick(AObject* object);
		void PostTick(AObject* object);
		void PreRender(AObject* object);
		void Render(AObject* object);
		void PostRender(AObject* object);
	};
}
