#include <SDL.h>
#include <vector>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 화면에 픽셀 찍는 함수
void PutPixel(std::vector<uint32_t>& buffer, int x, int y, uint32_t color) {
	// Todo 1: 범위 검사 작성
	// x나 y가 화면 범위보다 밖이라면, 함수 종료
	if (x >= WINDOW_WIDTH || x < 0 || y >= WINDOW_HEIGHT || y < 0) {
		return;
	}

	// Todo 2: 좌표 변환 공식 작성
	// buffer: 픽셀 데이터가 담긴 1차원 배열
	// 2차원 좌표를 1차원 배열 인덱스로 바꿔줘야 함
	// -> (y좌표값 * 화면 너비) + x좌표값
	int index = y * WINDOW_WIDTH + x;
	buffer[index] = color;
}

// 선 그리는 함수 (y = mx + b)
void DrawLine(std::vector<uint32_t>& buffer, int x1, int y1, int x2, int y2, uint32_t color) {
	// Todo 1: 기울기(m) 구하기
	float m = 0;

	// 세로선일 경우 (기울기가 무한대)
	// 공식을 사용하지 않고, y값을 1씩 늘려주며 찍어줌
	if ((x2 - x1) == 0) {
		if (y2 >= y1) {
			for (int y = y1; y <= y2; y++) {
				PutPixel(buffer, x1, y, color);
			}
		}
		else {
			for (int y = y2; y <= y1; y++) {
				PutPixel(buffer, x1, y, color);
			}
		}

		return;
	}
	// 그렇지 않은 경우, 공식을 사용
	m = static_cast<float>(y2 - y1) / static_cast<float>(x2 - x1);

	// Todo 2: y절편(b) 구하기
	float b = y1 - m * x1;

	// Todo 3: 점 찍기
	// x값을 x1부터 x2까지 1씩 증가시키면서 점을 찍어줌
	if (x2 > x1) {
		for (int x = x1; x <= x2; x++) {
			int y = m * x + b;
			PutPixel(buffer, x, y, color);
		}
	}
	else {
		for (int x = x2; x <= x1; x++) {
			int y = m * x + b;
			PutPixel(buffer, x, y, color);
		}
	}
}

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

		// 화면에 *모양의 빨간 직선 긋기 
		DrawLine(pixels, 0, 0, 800, 600, 0xFFFF0000);
		DrawLine(pixels, 0, 600, 800, 0, 0xFFFF0000);
		DrawLine(pixels, 400, 0, 400, 600, 0xFFFF0000);
		DrawLine(pixels, 0, 300, 800, 300, 0xFFFF0000);

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