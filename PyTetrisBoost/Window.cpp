#include "Window.h"

Window::Window() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"PyTetris",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		32 * 22, 32 * 22, 0
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Window::~Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Window::tick() {
	// Event Handle
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT)
			return 0;
		if (!handle_event(evt))
			return 0;
	}
	return 1;
}

int Window::handle_event(const SDL_Event& e) {

}