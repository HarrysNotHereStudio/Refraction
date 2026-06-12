#pragma once

#include <Core/Common.h>
#include <Core/Utilities.h>
#include <string>

namespace Refraction::Platform {
	class AWindow {
	public:
		Refraction::Enums::WindowInputState mInputState = Refraction::Enums::WindowInputState::NONE;
		struct {
			double lastMouseX = Settings::CurrentSettings->Window.Width / 2.0;
			double lastMouseY = Settings::CurrentSettings->Window.Height / 2.0;
			bool inputEnabled = false;
			bool inputFocus = false;
			bool keyW = false;
			bool keyA = false;
			bool keyS = false;
			bool keyD = false;
			bool keyQ = false;
			bool keyE = false;
			bool keyEsc = false;
			bool mouseLeft = false;
			bool mouseRight = false;
		} mInput;

		virtual ~AWindow() = default;

		virtual void Init() = 0;
		virtual void InitInput() = 0;
		virtual void OnUpdate() = 0;
		virtual void Cleanup() = 0;
		virtual uint32_t GetWidth() const { return mWidth; };
		virtual uint32_t GetHeight() const { return mHeight; };
		virtual void* GetNativeWindow() const = 0;
	protected:
		Refraction::Enums::WindowInputState mInputStateLast = Refraction::Enums::WindowInputState::NONE;
		uint32_t mWidth = 512;
		uint32_t mHeight = 512;
	};
}
