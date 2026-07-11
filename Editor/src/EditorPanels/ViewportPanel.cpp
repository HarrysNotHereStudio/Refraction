#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Rendering/RenderLayer.h>
#include <EditorState.h>

#include "ViewportPanel.h"

namespace Refraction::Editor::Panels {
	ViewportPanel::ViewportPanel(Common::Ref<Events::AEventDispatcher> eventDispatcher) : mEventDispatcher(eventDispatcher) {}

	inline void ViewportPanel::Init() {
	}

	void ViewportPanel::OnDraw() {
		ImGui::SetNextWindowSizeConstraints({ 640, 480 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse);

		if (mFirstDraw) {
			mLastViewportRect = Math::Rect(Math::FromImVec2(ImGui::GetWindowPos()), Math::FromImVec2(ImGui::GetContentRegionAvail()));
			mFirstDraw = false;
		}
		// Skip if no frame
		if (mFrame == nullptr) {
			ImGui::End();
			return;
		}

		ImGuiWindow* window = ImGui::GetCurrentWindow();

		mViewportRect = Math::Rect(Math::FromImVec2(ImGui::GetWindowPos()), Math::FromImVec2(ImGui::GetContentRegionAvail()));

		// On viewport resize
		if (mViewportRect.w != mLastViewportRect.w || mViewportRect.h != mLastViewportRect.h) {
			mEventDispatcher->Dispatch(Common::NewRef<Events::ViewportResizedEvent>(mViewportRect));
		}

		const float windowWidth = ImGui::GetContentRegionAvail().x;
		const float windowHeight = ImGui::GetContentRegionAvail().y;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 topLeft{ (float)mViewportRect.x, (float)mViewportRect.y };
		ImVec2 bottomRight{ (float)mViewportRect.x + (float)mViewportRect.w, (float)mViewportRect.y + (float)mViewportRect.h };
		ImVec2 imageSize = ImVec2(mFrame->GetMetadata().Width, mFrame->GetMetadata().Height);
		ImVec2 pos = ImGui::GetCursorScreenPos();
		drawList->AddImage((void*)mFrame->GetID(), ImVec2(pos.x, pos.y), ImVec2(pos.x + windowWidth, pos.y + windowHeight), ImVec2(0, 1), ImVec2(1, 0));
		

		// Don't update if RMB is down
		if (!ImGui::GetIO().MouseDown[1]) {
			EditorState::Temp.ViewportHovered = ImGui::IsWindowHovered();
		}

		ImGui::End();

		mLastViewportRect = mViewportRect;
	}
	inline void ViewportPanel::OnEvent(Common::Ref<Events::Event> event) {
		//Log::SInfo(event->GetName());
		if (auto e = Common::AsA<Events::FrameRenderedEvent>(event)) {
			mFrame = e->mFrame;
		}
	}
}
