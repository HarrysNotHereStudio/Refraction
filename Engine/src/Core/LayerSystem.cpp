#include "LayerSystem.h"


namespace Refraction::Engine {
	LayerStack::~LayerStack() {
		OnDetach();
	}

	void LayerStack::PushLayer(Common::Shared<ALayer> layer) {
		mLayers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Common::Shared<ALayer> layer) {}

	void LayerStack::OnPass() {
		for (auto& layer : mLayers) {
			layer->OnPass();
		}
	}

	void LayerStack::OnDetach() {
		for (auto& layer : mLayers) {
			layer->OnDetach();
		}
	}

	void LayerStack::Dispatch(Common::Shared<Events::Event> event) {
		for (auto& layer : mLayers) {
			layer->OnEvent(event);
			if (event->Consumed()) break;
		}
	}

}
