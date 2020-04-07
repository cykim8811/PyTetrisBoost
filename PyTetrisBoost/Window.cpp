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

int Window::update() {
	// Event Handle
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT)
			return 0;
		handle_event(evt);
	}
	// Draw Screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	draw_screen();
	SDL_RenderPresent(renderer);
	return 1;
}

void Window::handle_event(const SDL_Event& e) {
	
}

void Window::draw_screen() {

	SDL_Rect rect;

	for (int x = 0; x < state.map.w; x++) {
		for (int y = 0; y < state.map.h; y++) {
			int d = state.map.at(x, y);
			if (!d) continue;
			SDL_SetRenderDrawColor(renderer, TileColor[d - 1][0], TileColor[d - 1][1], TileColor[d - 1][2], 255);
			rect = SDL_Rect{ 32 * (6 + x), 32 * (1 + y), 32, 32 };
			SDL_RenderFillRect(renderer, &rect);
		}
	}


	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	for (int i = 0; i < 9; i++) {
		SDL_RenderDrawLine(renderer, 32 * (7 + i), 32 * 1, 32 * (7 + i), 32 * 21);
	}
	for (int i = 0; i < 19; i++) {
		SDL_RenderDrawLine(renderer, 32 * 6, 32 * (2 + i), 32 * 16, 32 * (2 + i));
	}


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	rect = { 32 * 6, 32 * 1, 32 * 10, 32 * 20 };
	SDL_RenderDrawRect(renderer, &rect);

	if (state.hold != -1) {
		draw_block(32 * 3, 32 * 3, state.hold, 28);
	}
	rect = { 32 * 1, 32 * 1, 32 * 4, 32 * 4 };
	SDL_RenderDrawRect(renderer, &rect);

	for (int i = 0; i < 5; i++) {
		draw_block(32 * 19, 32 * 1 + 24 * (2 + i * 5), state.next_block[i], 18);
		rect = { 32 * 19 - 24 * 2, 32 * 1 + 24 * (i * 5), 24 * 4, 24 * 4 };
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Window::draw_block(int x, int y, int type, int size) {

	Block* block = &Shape[type];
	int offset_x = x - int(block->size * size / 2),
		offset_y = y - int(block->size * size / 2);
	switch (type)
	{
	case 0:
		offset_y -= int(size / 2);
		break;
	case 1:
	case 2:
	case 4:
	case 5:
	case 6:
		offset_y += int(size / 2);
		break;
	default:
		break;
	}
	for (int ix = 0; ix < block->size; ix++) {
		for (int iy = 0; iy < block->size; iy++) {
			if (!block->at(ix, iy, 0)) continue;
			const int d = type;
			SDL_SetRenderDrawColor(renderer, TileColor[d][0], TileColor[d][1], TileColor[d][2], 255);
			SDL_Rect rect{ offset_x + size * ix, offset_y + size * iy, size, size };
			SDL_RenderFillRect(renderer, &rect);
			SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
			rect = { offset_x + size * ix, offset_y + size * iy, size + 1, size + 1 };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
}