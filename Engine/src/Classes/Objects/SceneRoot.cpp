#include <Core/Time.h>
#include <Classes/Components/AComponent.h>

#include "SceneRoot.h"

namespace Refraction::Objects {
	void SceneRoot::TickScene(std::vector<Common::Ref<AObject>> globalObjects) {
		PreTick(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			PreTick(objWeak.lock().get());
		}
		Tick(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			Tick(objWeak.lock().get());
		}
		PostTick(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			PostTick(objWeak.lock().get());
		}
	}

	void SceneRoot::RenderScene(std::vector<Common::Ref<AObject>> globalObjects) {
		PreRender(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			PreRender(objWeak.lock().get());
		}
		Render(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			Render(objWeak.lock().get());
		}
		PostRender(this);
		for (auto& objWeak : globalObjects) {
			if (objWeak.expired()) continue;
			PostRender(objWeak.lock().get());
		}
	}

	void SceneRoot::Remove(UUID target) {
		auto targetInst = GetInstanceWithUUID(target, this);
		if (!targetInst) return; // Target doesn't exist

		// If it's not the object, check components
		if (targetInst->GetUUID() != target) {
			for (auto& comp : *targetInst->GetComponents()) {
				if (comp->GetUUID() != target) continue;
				comp->mParent = nullptr;
				break;
			}
		} else {
			targetInst->mParent = nullptr;
		}
		Log::SInfo("Successfully removed " + target.AsString());
	}

	std::string SceneRoot::Serialise() {
		return AObject::Serialise();
	}
	void SceneRoot::Deserialise(std::string serialised) {
		AObject::Deserialise(serialised);
	}

	void SceneRoot::PreTick(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->PreTick();
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				PreTick(child.get());
			}
		}
	}
	void SceneRoot::Tick(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->Tick((float)Time::PhysicsDelta);
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				Tick(child.get());
			}
		}
	}
	void SceneRoot::PostTick(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->PostTick();
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				PostTick(child.get());
			}
		}
	}
	void SceneRoot::PreRender(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->PreRender();
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				PreRender(child.get());
			}
		}
	}
	void SceneRoot::Render(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->Render();
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				Render(child.get());
			}
		}
	}
	void SceneRoot::PostRender(AObject* object) {
		auto comps = object->GetComponents();
		for (auto& comp : *comps) {
			comp->PostRender();
		}
		auto children = object->GetChildren();
		if (children->size() > 0) {
			for (auto& child : *children) {
				PostRender(child.get());
			}
		}
	}
}
