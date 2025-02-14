#pragma once
#include <SDL.h>
#include <functional>

class Cube;

namespace UI
{
	struct Options
	{
		float angle = 20.f;
		float vel = 10.f;
		float mass = 10.f;
		float friction = 0.3f;
	};

	void Init(SDL_Window* window, SDL_Renderer* renderer);
	void ProcessEvent(SDL_Event event);
	void StartFrame();

	void DrawMenu(UI::Options* outOptions, std::function<void()> callback);
	void DrawStats(const Cube* cube);
	void EndFrame(SDL_Renderer* renderer);
	void Shutdown();

}