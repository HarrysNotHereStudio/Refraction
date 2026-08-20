#pragma once

#include <Platform/AMeshFragment.h>

namespace Refraction::Engine::Platform {
	class OpenGLMeshFragment : public AMeshFragment {
	public:
		using AMeshFragment::AMeshFragment;

		void Upload() override;
		void Draw() override;
	protected:
		unsigned int mVAO, mVBO, mEBO;
	};
}



