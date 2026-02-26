#include "graphics.h"
#include <cmath>
#include <algorithm>

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
	// Todo 1:  변화량 절댓값 구하기
	int dx = x2 - x1;
	int dy = y2 - y1;

	// Todo 2: 기울기 경사 비교
	// std::abs 함수는 cmath 헤더에 있음

	// 1) 기울기가 완만한 경우
	if (std::abs(dx) >= std::abs(dy)) {
		// 기울기(m) 구하기
		float m = static_cast<float>(dy) / static_cast<float>(dx);

		// 점 찍기 (x값을 x1부터 x2까지 1씩 증가시키면서 점을 찍어줌)
		if (x2 > x1) {
			for (int x = x1; x <= x2; x++) {
				int y = y1 + m * (x - x1);
				PutPixel(buffer, x, y, color);
			}
		}
		else {
			for (int x = x2; x <= x1; x++) {
				int y = y2 + m * (x - x2);
				PutPixel(buffer, x, y, color);
			}
		}
	}
	// 2) 기울기가 가파른 경우
	else {
		// 기울기 역수(invm) 구하기
		float invm = static_cast<float>(dx) / static_cast<float>(dy);

		// 점 찍기 (y값을 y1부터 y2까지 1씩 증가시키면서 점을 찍어줌)
		if (y2 > y1) {
			for (int y = y1; y <= y2; y++) {
				int x = x1 + invm * (y - y1);
				PutPixel(buffer, x, y, color);
			}
		}
		else {
			for (int y = y2; y <= y1; y++) {
				int x = x2 + invm * (y - y2);
				PutPixel(buffer, x, y, color);
			}
		}
	}
}

// 삼각형 그리는 함수
void DrawAndFilledTriangle(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// Todo 1: 삼각형의 각 꼭짓점 좌표 정렬

	// 1) y0 > y1 이면 두 점의 위치를 바꾼다. (아래로 갈 수록 값은 커짐)
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	// 2) y1 > y2 이면 두 점의 위치를 바꾼다.
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	// 3) 위에서 y0과 y1의 위치가 바뀌었을 수 있음
	//    따라서 y0, y1의 바뀐 위치를 한 번 더 비교
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}

	// Todo 2: 삼각형 쪼개기
	// y2 좌표에서 삼각형 내부를 가로지르는 선 하나를 일직선으로 그어 높이가 똑같은 점 M을 구한다.
	int My = y1;
	int Mx = 0;

	if (y2 - y0 == 0) { // 삼각형 높이가 0인 경우 예외처리
		Mx = x1;
	}
	else { // 선형 보간 공식 사용
		Mx = x0 + static_cast<int>(static_cast<float>(x2 - x0) * (y1 - y0) / (y2 - y0));
	}

	// Todo 3: 위쪽 삼각형 채우기

	// 1) 역기울기 구하기
	float inv_slope1 = 0;
	float inv_slope2 = 0;

	if (y1 - y0 != 0) {
		inv_slope1 = static_cast<float>(x1 - x0) / (y1 - y0);
	}
	if (y2 - y0 != 0) {
		inv_slope2 = static_cast<float>(x2 - x0) / (y2 - y0);
	}

	// 2) 시작점 설정
	float cur_x1 = static_cast<float>(x0);
	float cur_x2 = static_cast<float>(x0);

	// 3) 한 줄씩 그리기
	for (int y = y0; y < y1; y++) {
		int start_x = static_cast<int>(std::min(cur_x1, cur_x2));
		int end_x = static_cast<int>(std::max(cur_x1, cur_x2));

		DrawLine(buffer, start_x, y, end_x, y, color);

		cur_x1 += inv_slope1;
		cur_x2 += inv_slope2;
	}

	// Todo 4: 아래쪽 삼각형 채우기

	// 1) y2 좌표에서 꺾이며 생기는 새 기울기 구하기
	float inv_slope3 = 0;

	if (y2 - y1 != 0) {
		inv_slope3 = static_cast<float>(x2 - x1) / (y2 - y1);
	}

	// 2) cur_x1, cur_x2 위치 다시 조정 
	cur_x1 = static_cast<float>(x1);
	cur_x2 = static_cast<float>(Mx);

	// 3) 한 줄씩 그리기
	for (int y = y1; y <= y2; y++) {
		int start_x = static_cast<int>(std::min(cur_x1, cur_x2));
		int end_x = static_cast<int>(std::max(cur_x1, cur_x2));

		DrawLine(buffer, start_x, y, end_x, y, color);

		cur_x1 += inv_slope3;
		cur_x2 += inv_slope2;
	}
}