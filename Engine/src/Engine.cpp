#include <iostream>
#include <map>
#include <thread>
#include <string>

#include <Core/Common.h>
#include <Core/Log.h>
#include <Core/Utilities.h>
#include <Rendering/Renderer.h>

#include "Engine.h"

Settings* Settings::CurrentSettings = new Settings();

namespace {
	void RunRenderer() {
		Renderer* renderer = Renderer::GetInstance();
		try {
			renderer->SetResourcePath(Refraction::Constants::GetResourcePath());
			renderer->Init();
			renderer->Run();
		} catch (const std::exception& e) {
			std::cerr << e.what() << '\n';
			Refraction::Log::Error(e.what());
			Renderer::DestroyInstance();
			throw;
		}

		Renderer::DestroyInstance();
	}
}

void RefractionEngine::InitWindow() {
	Renderer::GetInstance()->InitWindow();
}

std::thread RendererThread;
void RefractionEngine::Init() {
	std::cout << "Initialising Refraction Engine...\n";

	Settings::CurrentSettings->Window.Width = 1920;
	Settings::CurrentSettings->Window.Height = 1080;
	Settings::CurrentSettings->Graphics.ViewportWidth = 1920;
	Settings::CurrentSettings->Graphics.ViewportHeight = 1080;
	Settings::CurrentSettings->Controls.CameraSensitivity = 0.5f;
	Settings::CurrentSettings->Controls.CameraSpeed = 0.5f;

	try {
		Refraction::Log::Info("Resource path: " + Refraction::Constants::GetResourcePath());
		Refraction::Log::Info("Starting render thread...");
		Renderer* renderer = Renderer::GetInstance();
		RunRenderer();
		RendererThread = std::thread(RunRenderer);

		// Wait for renderer to initialise
		while (renderer->GetState() != RendererState::RUNNING) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Allow inputs to be processed
		Refraction::Log::Info("Starting input thread...");
		renderer->GetWindow()->InitInput();

	} catch (...) {
		std::cout << "Error occurred during engine initialisation, aborting\n\n";
	}
	return;
}

bool RefractionEngine::IsRunning() {
	return (Renderer::GetInstance()->GetState() == RendererState::RUNNING);
}

void RefractionEngine::Shutdown() {
	if (!RendererThread.joinable()) return;
	Renderer* renderer = Renderer::GetInstance();

	// Wait for renderer to clean up before exiting
	while (renderer->GetState() != RendererState::EXIT) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	RendererThread.join();

	std::cout << "\n\n--------------------------------\n";
	std::cout << "Refraction shut down successfully.\n\n";
}

void RefractionEngine::SetResourcePath(std::string path) {
	Refraction::Constants::ResourcePath = path;
	Refraction::Log::Info("Set Resource path to " + Refraction::Constants::GetResourcePath());
}

void RefractionEngine::SetViewport(int x, int y, int w, int h) {
	Renderer::GetInstance()->SetViewport(Refraction::Math::Rect(x, y));
}

void RefractionEngine::SetEditorInterfaceDrawCallback(std::function<void()> callback) {
	Renderer::GetInstance()->SetEditorInterfaceDrawCallback(callback);
}
