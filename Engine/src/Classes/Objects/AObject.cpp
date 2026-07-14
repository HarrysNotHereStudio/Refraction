#include <json.hpp>

#include <Classes/Components/AComponent.h>
#include <Classes/ClassSerialiser.h>

#include "AObject.h"


namespace Refraction::Objects {
	AObject::AObject(const AObject& object) {
		mTransform = object.mTransform;
		mInstanceName = object.mInstanceName;
		mParent = object.mParent;
		mUUID = UUID();

		// Deep copy components and children
		for (auto& comp : object.mComponents) {
			Common::Ref<Components::AComponent> newComp(new Components::AComponent(*comp));
			newComp->mParent = this;
			mComponents.push_back(newComp);
		}

		for (auto& child : object.mChildren) {
			Common::Ref<AObject> newChild(new AObject(*child));
			newChild->mParent = this;
			mChildren.push_back(newChild);
		}
	}

	AObject::~AObject() {
		mUUID.Reset();
	}

	AObject* AObject::GetFirstChild(std::string name) {
		for (auto& obj : mChildren) {
			if (obj->mInstanceName == name) return obj.get();
		}
		return nullptr;
	}

	void AObject::AddChild(Common::Ref<AObject> child) {
		if (child->mParent == this) return; // Child is already parented to this object
		child->mParent = this;
		mChildren.push_back(child);
	}

	Common::Ref<AObject> AObject::Clone() {
		return Common::Ref<AObject>(new AObject(*this));
	}

	std::string AObject::Serialise() {
		using nlohmann::json;
		json serialised;
		serialised["UUID"] = mUUID.Serialise();
		serialised["TypeName"] = typeid(*this).name();
		Log::SInfo("Serialising as " + std::string(typeid(*this).name()));
		serialised["DisplayName"] = mDisplayName;
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
			mDisplayName = json.at("DisplayName").get<std::string>();
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
}
