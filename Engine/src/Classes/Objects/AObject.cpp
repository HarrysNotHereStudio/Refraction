#include <json.hpp>

#include <Classes/Components/AComponent.h>
#include <Classes/ClassSerialiser.h>

#include "AObject.h"


namespace Refraction::Objects {
	AObject* AObject::GetInstanceWithUUID(UUID target, AObject* parent) {
		for (auto& comp : *parent->GetComponents()) {
			if (comp->GetUUID() == target) return parent;
		}
		AObject* obj = nullptr;
		for (auto& child : *parent->GetChildren()) {
			obj = GetInstanceWithUUID(target, child.get());
		}
		return obj;
	}

	AObject::AObject(const AObject& object) {
		mTransform = object.mTransform;
		mInstanceName = object.mInstanceName;
		mParent = object.mParent;
		mUUID = UUID();

		// Deep copy components and children
		for (auto& comp : object.mComponents) {
			Common::SRef<Components::AComponent> newComp(new Components::AComponent(*comp));
			newComp->mParent = this;
			mComponents.push_back(newComp);
		}

		for (auto& child : object.mChildren) {
			Common::SRef<AObject> newChild(new AObject(*child));
			newChild->mParent = this;
			mChildren.push_back(newChild);
		}
	}

	AObject::~AObject() {
		mUUID.Reset();
	}

	Common::SRef<AObject> AObject::GetFirstChild(std::string name) {
		for (auto& obj : mChildren) {
			if (obj->mInstanceName == name) return obj;
		}
		return nullptr;
	}

	void AObject::AddChild(Common::SRef<AObject> child) {
		if (child->mParent == this) return; // Child is already parented to this object
		child->mParent = this;
		mChildren.push_back(child);
	}

	void AObject::Remove() {
		if (!mParent) return;
		mParent->RemoveChild(mUUID);
	}

	void AObject::RemoveChild(UUID target) {
		for (size_t i = 0; i < mChildren.size(); i++) {
			auto& child = mChildren[i];
			if (child->GetUUID() != target) continue;
			child->mParent = nullptr;
			mChildren.erase(std::next(mChildren.begin(), i));
			return;
		}

		// It isn't a child object, try components
		for (size_t i = 0; i < mComponents.size(); i++) {
			auto& comp = mComponents[i];
			if (comp->GetUUID() != target) continue;
			comp->mParent = nullptr;
			mComponents.erase(std::next(mComponents.begin(), i));
			return;
		}

		// Well it isn't a child component either. Get mad at the caller.
	}

	Common::SRef<AObject> AObject::Clone() {
		return Common::SRef<AObject>(new AObject(*this));
	}

	std::string AObject::Serialise() {
		using nlohmann::json;
		json serialised;
		serialised["UUID"] = mUUID.Serialise();
		serialised["TypeName"] = typeid(*this).name();
		Log::SInfo("Serialising as " + std::string(typeid(*this).name()));
		serialised["ClassName"] = mClassName;
		serialised["InstanceName"] = mInstanceName;
		serialised["Transform"] = Utilities::ClassSerialiser::Serialise(mTransform);
		serialised["Components"] = {};
		for (auto& comp : mComponents) {
			serialised["Components"][comp->GetUUID().Serialise()] = comp->Serialise();
		}
		serialised["Children"] = {};
		for (auto& child : mChildren) {
			serialised["Children"][child->GetUUID().Serialise()] = child->Serialise();
		}

		Log::SInfo("Serialised object " + mInstanceName + " with UUID " + mUUID.AsString());
		return serialised.dump();
	}
	void AObject::Deserialise(std::string serialised) {
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mUUID = UUID::Deserialise(json.at("UUID"));
			mClassName = json.at("ClassName").get<std::string>();
			mInstanceName = json.at("InstanceName").get<std::string>();
			mTransform = Utilities::ClassSerialiser::DeserialiseTransform(json.at("Transform"));
			mComponents.clear();
			for (auto& compData : json.at("Components")) {
				auto newComp = Utilities::ClassSerialiser::DeserialiseComponent(compData);
				newComp->mParent = this;
				mComponents.push_back(newComp);
			}
			mChildren.clear();
			for (auto& childData : json.at("Children")) {
				auto newChild = Utilities::ClassSerialiser::DeserialiseObject(childData);
				newChild->mParent = this;
				mChildren.push_back(newChild);
			}

			Log::SInfo("Deserialised object " + mInstanceName + " with UUID " + mUUID.AsString());
		});
	}

	Math::Transform AObject::GetWorldTransform() {
		auto transform = mTransform;
		if (mParent) {
			auto result = mTransform.ToMatrix() * mParent->GetWorldTransform().ToMatrix();
			transform = Math::Transform::FromMatrix(result);
		}
		return transform;
	}
}
