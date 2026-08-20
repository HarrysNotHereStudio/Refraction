#include "PhysicsLayer.h"

namespace Refraction::Engine {
	PhysicsLayer::PhysicsLayer(Common::SRef<Events::AEventDispatcher> eventDispatcher, Common::SRef<Project> projectInstance)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance) {}

	void PhysicsLayer::OnAttach() {
	}

	void PhysicsLayer::OnDetach() {
	}

	void PhysicsLayer::OnPass() {
		if (mProjectInstance->IsLoaded()) {
			if (auto scene = mProjectInstance->GetActiveScene()) {
				scene->TickScene(mProjectInstance->GetGlobalObjects());
			}
		}
	}

	void PhysicsLayer::OnEvent(Common::SRef<Events::Event> event) {
	}
}
