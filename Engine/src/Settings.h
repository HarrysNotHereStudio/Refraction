#pragma once

class Settings {
public:
	static Settings* CurrentSettings;

	struct {
		int ViewportWidth = 1280;
		int ViewportHeight = 720;
		int MaxFrameRate = 0;
		bool WireframeEnabled = false;
	} Graphics;

	struct {
		const char* Title = "Refraction";
		int Width = 1280;
		int Height = 720;
	} Window;

	struct {
		float CameraSensitivity = 0.5f;
		float CameraSpeed = 0.05f;
	} Controls;
};