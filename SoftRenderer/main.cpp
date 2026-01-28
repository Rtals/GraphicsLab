#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
	// 1. SDL 초기화
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL 초기화 실패! Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// 2. 윈도우 생성 (800x600 해상도)
	SDL_Window* window = SDL_CreateWindow(
		"MyLab - SoftRenderer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		std::cout << "윈도우 생성 실패! Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// 3. 3초간 대기 (창이 떴는지 확인용)
	SDL_Delay(3000);

	// 4. 종료
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}