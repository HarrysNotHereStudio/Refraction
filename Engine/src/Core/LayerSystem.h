#pragma once

#include <vector>

#include <Core/Common.h>
#include <Core/EventSystem.h>

namespace Refraction::Events {
	class ProgramCloseEvent : public Events::Event {
	public:
		ProgramCloseEvent() {
			mName = "ProgramClose";
		}
	};
}

namespace Refraction::Engine {

	class ALayer {
	public:
		virtual void OnAttach() {}
		virtual void OnPass() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Common::SRef<Events::Event> event) {}
	};

	class LayerStack : public Events::AEventDispatcher {
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Common::SRef<ALayer> layer);
		void PopLayer(Common::SRef<ALayer> layer);

		void OnPass();
		void OnDetach();
		virtual void Dispatch(Common::SRef<Events::Event> event) override;
	private:
		std::vector<Common::SRef<ALayer>> mLayers;
	};
}