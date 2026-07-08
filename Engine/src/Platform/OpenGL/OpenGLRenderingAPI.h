#pragma once

#include <Core/Common.h>
#include <Math/Vector4.h>
#include <Math/Rect.h>
#include <Platform/ARenderingAPI.h>

namespace Refraction::Engine::Platform {
	class OpenGLRenderingAPI : public ARenderingAPI {
	public:
		void Init() override;
		void Clear(const Math::Vector4 colour) override;
		void SetViewportRect(Math::Rect rect) override;

	};
}
