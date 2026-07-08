#pragma once

#include <Core/Common.h>
#include <Core/LayerSystem.h>
#include <Platform/AWindow.h>
#include <Platform/ARenderingAPI.h>
#include <Rendering/RenderLayer.h>
#include <Physics/PhysicsLayer.h>
#include <Interface/Project.h>

namespace Refraction::Engine {
	class Instance {
	public:
		Instance(Platform::WindowProperties windowProps = Platform::WindowProperties());
		virtual ~Instance() = default;
		void Start();
		void End();

	protected:
		Common::Ref<Platform::AWindow> mWindow;
		Common::Ref<LayerStack> mLayerStack;
		Common::Ref<Platform::ARenderingAPI> mRenderingAPI;
		Common::Ref<RenderLayer> mRenderLayer;
		Common::Ref<PhysicsLayer> mPhysicsLayer;
		Common::Ref<Project> mProjectInstance;
	};
}
