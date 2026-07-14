#include "RenderLayer.h"

namespace Refraction::Engine {
	RenderLayer::RenderLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Project> projectInstance)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance) {}

	void RenderLayer::OnAttach() {
		mRenderer.Init();
	}
	void RenderLayer::OnDetach() {}

	void RenderLayer::OnPass() {
		if (mProjectInstance->IsLoaded()) {
			mRenderer.RenderFrame(mProjectInstance);
			mEventDispatcher->Dispatch(Common::NewRef<Events::FrameRenderedEvent>(mRenderer.GetFinalOutput()));
		}
	}

	void RenderLayer::OnEvent(Common::Ref<Events::Event> event) {
		// Update renderer for a resized viewport
		if (auto e = Common::AsA<Events::ViewportResizedEvent>(event)) {
			mRenderer.SetViewport(Math::Rect(e->mViewportRect.x, e->mViewportRect.y, e->mViewportRect.w, e->mViewportRect.h));
		} else if (auto e = Common::AsA<Events::ProgramCloseEvent>(event)) {

		}
	}
}
