#include "PhysicsLayer.h"

namespace Refraction::Engine {
	PhysicsLayer::PhysicsLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Project> projectInstance)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance) {}

	void PhysicsLayer::OnAttach() {
	}

	void PhysicsLayer::OnDetach() {
	}

	void PhysicsLayer::OnPass() {
		if (mProjectInstance->IsLoaded()) {
			if (auto scene = mProjectInstance->GetActiveScene()) {
				scene->TickScene();
			}
		}
	}

	void PhysicsLayer::OnEvent(Common::Ref<Events::Event> event) {
	}
}
