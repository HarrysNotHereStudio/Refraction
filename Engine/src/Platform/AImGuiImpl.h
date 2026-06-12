#pragma once

#include <string>
#include <deque>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Core/Common.h>
#include <Math/Vector3.h>
#include <EngineClasses/Objects/AObject.h>

constexpr int ImGuiImpl_DeltaHistoryMax = 90;

namespace Refraction::Platform {
	class AImGuiImpl {
	public:
		struct {
			std::deque<float> deltaHistory = {};
			float fps = 0;
			float deltaTime = 0;
			float elapsedTime = 0;
			int meshCount = 0;
			int vertexCount = 0;
			std::string inputStateStr = "None";
			Math::Vector3 cameraGridIndex = Math::Vector3(0);
			Math::Vector3 cameraCellPos = Math::Vector3(0.0f);
			Math::Vector3 cameraWorldPos = Math::Vector3(0.0f);
		} mDebugValues;
		Common::Ref<Objects::AObject> mSelectedObject = nullptr;

		static ImGuiStyle GetDefaultStyle();

		AImGuiImpl();

		void Draw();
		void GetGuiInputState(Refraction::Enums::WindowInputState* inputState);
		void HideMouse();
	protected:
		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

	private:
		void DrawDebugInfoWindow();
		void DrawSelectedTransformControl();
		void DrawSelectedComponentControls();
	};
}