#pragma once
#include "SDL.h"
#include "State.h"

class Window {
public:
	Window();
	~Window();
	int update(); // 0: end, 1: normal
	State state;
private:
	SDL_Renderer* renderer;
	SDL_Window* window;

	void handle_event(const SDL_Event& e);
	void draw_screen();
	void draw_block(int x, int y, int type, int size);

};

