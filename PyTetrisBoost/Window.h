#pragma once
#include "SDL.h"

class Window {
public:
	Window();
	~Window();
	int tick(); // 0: end, 1: normal
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

	int handle_event(const SDL_Event& e);
};

