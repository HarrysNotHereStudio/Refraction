#include <Core/Time.h>
#include <Classes/Components/AComponent.h>

#include "SceneRoot.h"

namespace Refraction::Objects {
	void SceneRoot::TickScene() {
		PreTick(this);
		Tick(this);
		PostTick(this);
	}

	void SceneRoot::RenderScene() {
		PreRender(this);
		Render(this);
		PostRender(this);
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
