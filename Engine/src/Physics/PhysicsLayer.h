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
		PhysicsLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Project> projectInstance);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::Ref<Events::Event> event) override;
	private:
		Common::Ref<Events::AEventDispatcher> mEventDispatcher;
		Common::Ref<Project> mProjectInstance;
	};
}
