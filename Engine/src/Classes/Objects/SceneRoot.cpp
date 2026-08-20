#include <Core/Time.h>
#include <Classes/Components/AComponent.h>

#include "SceneRoot.h"

namespace Refraction::Objects {
	void SceneRoot::TickScene(std::vector<Common::SRef<AObject>> globalObjects) {
		PreTick(this);
		for (auto& obj : globalObjects) PreTick(obj.get());
		Tick(this);
		for (auto& obj : globalObjects) Tick(obj.get());
		PostTick(this);
		for (auto& obj : globalObjects) PostTick(obj.get());
	}

	void SceneRoot::RenderScene(std::vector<Common::SRef<AObject>> globalObjects) {
		PreRender(this);
		for (auto& obj : globalObjects) PreRender(obj.get());
		Render(this);
		for (auto& obj : globalObjects) Render(obj.get());
		PostRender(this);
		for (auto& obj : globalObjects) PostRender(obj.get());
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
