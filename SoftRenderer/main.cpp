#include <SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "graphics.h"
#include "math.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Pixel Plotting",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH, WINDOW_HEIGHT);

	std::vector<uint32_t> pixels(WINDOW_WIDTH * WINDOW_HEIGHT, 0);

	bool isRunning = true;
	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		// 화면을 검은색으로 초기화
		std::fill(pixels.begin(), pixels.end(), 0xFF000000);

		// 중앙에 빨간 점 찍기
		PutPixel(pixels, 400, 300, 0xFFFF0000);

		// 화면에 빨간 직선 긋기 
		DrawLine(pixels, 0, 0, 60, 600, 0xFFFF0000);
		DrawLine(pixels, 0, 0, 800, 80, 0xFFFF0000);
		DrawLine(pixels, 100, 0, 100, 600, 0xFFFF0000);
		DrawLine(pixels, 0, 100, 800, 100, 0xFFFF0000);

		// 화면에 녹색 삼각형 그리기
		DrawAndFilledTriangle(pixels, 400, 100, 100, 300, 600, 500, 0xFF00FF00);

		SDL_UpdateTexture(texture, NULL, pixels.data(), WINDOW_WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

