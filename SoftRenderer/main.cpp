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

	float theta = 0; // 회전 각도

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		// 화면을 검은색으로 초기화
		std::fill(pixels.begin(), pixels.end(), 0xFF000000);

		// 회전 행렬 생성 (현재 Y축만 사용)
		Mat4x4 matRotZ = Mat4x4::makeRotationZ(theta);
		Mat4x4 matRotX = Mat4x4::makeRotationX(theta);
		Mat4x4 matRotY = Mat4x4::makeRotationY(theta);

		// 큐브의 모든 삼각형 순회
		for (auto tri : cubeMesh.tris) {
			Triangle projectedTri;

			for (int i = 0; i < 3; i++) {
				// 큐브가 원점을 중심으로 돌아가도록 큐브의 중심을 원점으로 조정
				tri.p[i].x -= 0.5;
				tri.p[i].y -= 0.5;
				tri.p[i].z -= 0.5;

				// 삼각형의 각 꼭짓점 좌표를 Z축 -> X축 순서로 회전
				Vec3 rotated_vec = matRotY.multiplyVector(tri.p[i]);

				// Z축으로 밀어넣어 카메라에서 멀어지도록 조정 (Z값이 0이면 카메라 위치와 겹침)
				rotated_vec.z += 3;

				// 2D 화면으로 투영 (원근법 적용)
				projectedTri.p[i].x = rotated_vec.x / rotated_vec.z;
				projectedTri.p[i].y = rotated_vec.y / rotated_vec.z;

				// 화면 중앙으로 좌표 옮기기 & 크기 키우기 (투영된 값이 매우 작기 때문에)
				projectedTri.p[i].x = (projectedTri.p[i].x + 1) * 0.5 * WINDOW_WIDTH;
				projectedTri.p[i].y = (projectedTri.p[i].y + 1) * 0.5 * WINDOW_HEIGHT;
			}

			// 삼각형 한 면 그리기
			DrawLine(pixels, projectedTri.p[0].x, projectedTri.p[0].y, projectedTri.p[1].x, projectedTri.p[1].y, 0xFFFFFFFF);
			DrawLine(pixels, projectedTri.p[1].x, projectedTri.p[1].y, projectedTri.p[2].x, projectedTri.p[2].y, 0xFFFFFFFF);
			DrawLine(pixels, projectedTri.p[2].x, projectedTri.p[2].y, projectedTri.p[0].x, projectedTri.p[0].y, 0xFFFFFFFF);
		}

		// 다음 프레임을 위해 각도 증가
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