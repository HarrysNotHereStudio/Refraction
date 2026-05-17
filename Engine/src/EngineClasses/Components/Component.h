#pragma once

#include <string>

class Component {
public:
	Component();
	~Component();

	virtual void PreTick() {};
	virtual void Tick() {};
	virtual void PostTick() {};
	virtual void PreRender() {};
	virtual void Render() {};
	virtual void PostRender() {};

	std::string GetUUID() const { return mUUID; };

protected:
	std::string mDisplayName = "BaseComponent";
private:
	std::string mUUID;
};
