#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <Core/LayerSystem.h>
#include <Math/Rect.h>
#include <Classes/Assets/Image.h>
#include <Rendering/Renderer.h>
#include <Interface/Project.h>

namespace Refraction::Events {
	class FrameRenderedEvent : public Events::Event {
	public:
		Common::Ref<Assets::Image> mFrame;
		FrameRenderedEvent(Common::Ref<Assets::Image> newFrame) : mFrame(newFrame) {
			mName = "FrameRendered";
		}
	};
	class ViewportResizedEvent : public Events::Event {
	public:
		Math::Rect mViewportRect;
		ViewportResizedEvent(Math::Rect newRect) : mViewportRect(newRect) {
			mName = "ViewportResized";
		}
	};
}

namespace Refraction::Engine {
	class RenderLayer : public ALayer {
	public:
		RenderLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Project> projectInstance);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::Ref<Events::Event> event) override;
	private:
		Common::Ref<Events::AEventDispatcher> mEventDispatcher;
		Common::Ref<Project> mProjectInstance;

		Renderer mRenderer;
	};
}


