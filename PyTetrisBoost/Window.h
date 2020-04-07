#pragma once
#include "SDL.h"

class Window {
public:
	Window();
	~Window();
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
};

