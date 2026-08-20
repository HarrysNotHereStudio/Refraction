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
		Common::SRef<Platform::AWindow> mWindow;
		Common::SRef<LayerStack> mLayerStack;
		Common::SRef<Platform::ARenderingAPI> mRenderingAPI;
		Common::SRef<RenderLayer> mRenderLayer;
		Common::SRef<PhysicsLayer> mPhysicsLayer;
		Common::SRef<Project> mProjectInstance;
	};
}
