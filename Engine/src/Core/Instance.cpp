#include "Instance.h"

Settings* Settings::CurrentSettings = new Settings();

namespace Refraction::Engine {
	Instance::Instance(Platform::WindowProperties windowProps) {
		Log::InitConsoleLog();
		mWindow = Platform::AWindow::Get();
		mWindow->Init();
		mLayerStack = Common::NewRef<LayerStack>();
		mProjectInstance = Common::NewRef<Project>();
		mRenderingAPI = Platform::ARenderingAPI::Get();
		mRenderingAPI->Init();
		mRenderLayer = Common::NewRef<RenderLayer>(mLayerStack, mProjectInstance);
		mLayerStack->PushLayer(mRenderLayer);
		mPhysicsLayer = Common::NewRef<PhysicsLayer>(mLayerStack, mProjectInstance);
		mLayerStack->PushLayer(mPhysicsLayer);
	}

	void Instance::Start() {
		mWindow->InitInput();
		while (!mWindow->ShouldClose()) {
			mWindow->OnUpdate(mProjectInstance->GetActiveCamera());
			if (mWindow->mShouldFramebufferRegen) {
				mLayerStack->Dispatch(Common::NewRef<Events::ViewportResizedEvent>(mWindow->GetRect()));
				mWindow->mShouldFramebufferRegen = false;
			}
			mRenderingAPI->Clear(Math::Vector4(0, 0, 0, 1));
			mLayerStack->OnPass();
		}
	}

	void Instance::End() {
		Log::SInfo("Shutting down instance");
		if (mProjectInstance->IsLoaded()) {
			mProjectInstance->Save();
			mProjectInstance->Close();
		}
		mLayerStack->OnDetach();
	}
}
