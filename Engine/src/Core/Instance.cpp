#include "Instance.h"

Settings* Settings::CurrentSettings = new Settings();

namespace Refraction::Engine {
	Instance::Instance(Platform::WindowProperties windowProps) {
		try {
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
		} catch (const std::runtime_error& err) {
			Log::SError("Critical error encountered during startup: " + std::string(err.what()));
		} catch(...) {
			Log::SError("Unknown critical error encountered during startup");
			throw;
		}
	}

	void Instance::Start() {
		try {
			mWindow->InitInput();
			while (!mWindow->ShouldClose()) {
				mWindow->OnUpdate(Objects::Camera::ActiveCamera);
				if (mWindow->mShouldFramebufferRegen) {
					mLayerStack->Dispatch(Common::NewRef<Events::ViewportResizedEvent>(mWindow->GetRect()));
					mWindow->mShouldFramebufferRegen = false;
				}
				mRenderingAPI->Clear(Math::Vector4(0, 0, 0, 1));
				mLayerStack->OnPass();
			}
		} catch (const std::runtime_error& err) {
			Log::SError("Critical error encountered during runtime: " + std::string(err.what()));
		} catch (...) {
			Log::SError("Unknown critical error encountered during runtime");
		}
	}

	void Instance::End() {
		try {
			Log::SInfo("Shutting down instance");
			// Save and close project just in case, user would normally close from GUI
			if (mProjectInstance->IsLoaded()) {
				mProjectInstance->Save();
				mProjectInstance->Close();
			}
			mLayerStack->OnDetach();
		} catch (const std::runtime_error& err) {
			Log::SError("Critical error encountered during shutdown: " + std::string(err.what()));
		} catch (...) {
			Log::SError("Unknown critical error encountered during shutdown");
		}
	}
}
