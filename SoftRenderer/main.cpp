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

	// 3D 큐브 데이터 (정육면체 -> 12개의 삼각형으로 구현)
	Mesh cubeMesh;
	cubeMesh.tris = {
		// 남쪽 면 (South Face)
		{ { {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} } },
		{ { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} } },

		// 동쪽 면 (East Face)
		{ { {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} } },
		{ { {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f} } },

		// 북쪽 면 (North Face)
		{ { {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f} } },
		{ { {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} } },

		// 서쪽 면 (West Face)
		{ { {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} } },
		{ { {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} } },

		// 위쪽 면 (Top Face)
		{ { {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} } },
		{ { {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} } },

		// 아래쪽 면 (Bottom Face)
		{ { {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} } },
		{ { {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} } }
	};

	// 색상
	Color color;

	// 회전 각도
	float theta = 0;

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		// 화면 검정색으로 채우기
		FillScreen(pixels, color.black);

		// 회전 행렬 생성 (현재 Y축만 사용)
		Mat4x4 matRotY = Mat4x4::makeRotationY(theta);

		// 3D 큐브 그리기
		DrawMesh(pixels, cubeMesh, matRotY, color.red);
		
		// 다음 프레임(회전)을 위해 각도 증가
		theta += 0.005;

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
