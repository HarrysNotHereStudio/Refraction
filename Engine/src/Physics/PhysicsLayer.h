#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <Core/LayerSystem.h>
#include <Interface/Project.h>

namespace Refraction::Events {
}

namespace Refraction::Engine {
	class PhysicsLayer : public ALayer {
	public:
		PhysicsLayer(Common::SRef<Events::AEventDispatcher> eventDispatcher, Common::SRef<Project> projectInstance);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::SRef<Events::Event> event) override;
	private:
		Common::SRef<Events::AEventDispatcher> mEventDispatcher;
		Common::SRef<Project> mProjectInstance;
	};
}
